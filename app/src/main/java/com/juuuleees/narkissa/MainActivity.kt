package com.juuuleees.narkissa

import android.Manifest
import android.annotation.SuppressLint
import android.bluetooth.BluetoothAdapter
import android.bluetooth.BluetoothDevice
import android.bluetooth.BluetoothManager
import android.bluetooth.BluetoothServerSocket
import android.bluetooth.BluetoothSocket
import android.content.BroadcastReceiver
import android.content.ContentValues
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.content.pm.PackageManager
import android.os.Build
import android.os.Bundle
import android.provider.MediaStore
import android.util.Log
import android.widget.Toast
import androidx.activity.result.contract.ActivityResultContracts
import androidx.appcompat.app.AppCompatActivity
import androidx.camera.core.CameraSelector
import androidx.camera.core.ImageAnalysis
import androidx.camera.core.ImageProxy
import androidx.camera.core.Preview
import androidx.camera.lifecycle.ProcessCameraProvider
import androidx.camera.video.MediaStoreOutputOptions
import androidx.camera.video.Quality
import androidx.camera.video.QualitySelector
import androidx.camera.video.Recorder
import androidx.camera.video.Recording
import androidx.camera.video.VideoCapture
import androidx.camera.video.VideoRecordEvent
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import com.juuuleees.narkissa.databinding.ActivityMainBinding
import java.io.IOException
import java.nio.ByteBuffer
import java.text.SimpleDateFormat
import java.util.Locale
import java.util.UUID
import java.util.concurrent.ExecutorService
import java.util.concurrent.Executors

typealias LumaListener = (luma: Double) -> Unit

class MainActivity : AppCompatActivity() {
    private lateinit var  viewBinding: ActivityMainBinding

    private var videoCapture: VideoCapture<Recorder>? = null
    private var recording: Recording? = null
    private lateinit var bluetoothAdapter: BluetoothAdapter
//    private lateinit var btServerSocket: BluetoothServerSocket
    private lateinit var cameraExecutor: ExecutorService
    private var permissionsGranted = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        viewBinding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(viewBinding.root)

//        request camera permissions
        if (allPermissionsGranted()) {
            permissionsGranted = true
            startCamera()
        } else {
            requestPermissions()
        }

        val bluetoothAvailable = packageManager.hasSystemFeature(PackageManager.FEATURE_BLUETOOTH)
        val bluetoothManager: BluetoothManager = getSystemService(BluetoothManager::class.java)
        val bluetoothRequestCode = 1
        val discoverableIntent: Intent = Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE)
            .apply {
                putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 120)
            }
        bluetoothAdapter = bluetoothManager.getAdapter()

        if ((bluetoothAvailable == true ) && (bluetoothAdapter.isEnabled == false)) {
            val enableBluetoothIntent = Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE)
            val filter = IntentFilter(BluetoothDevice.ACTION_FOUND)
            registerReceiver(receiver, filter)
        }

//        set up listener for start_connection button
        viewBinding.bluetoothConnectButton.setOnClickListener { bluetoothToArduino() }
        viewBinding.videoCaptureButton.setOnClickListener { captureVideo() }

        cameraExecutor = Executors.newSingleThreadExecutor()

        startActivityForResult(discoverableIntent, bluetoothRequestCode)
    }

    private fun bluetoothToArduino() {
        viewBinding.bluetoothConnectButton.isEnabled = false
        Log.d(TAG, "Trying to connect to HC-05...")

        class AcceptThread: Thread() {

            // fixed permission error 5-11-2024
            @SuppressLint("MissingPermission")
            val btServerSocket: BluetoothServerSocket? = bluetoothAdapter?.
            listenUsingInsecureRfcommWithServiceRecord(NAME, NB_UUID)

            override fun run() {
                var shouldLoop = true
                while (shouldLoop) {
                    val socket: BluetoothSocket? = try {
                        btServerSocket?.accept()
                    } catch (ioe: IOException) {
                        Log.e(TAG, "Socket accept() method failed")
                        shouldLoop = false
                        null
                    }
                    socket?.also {
                        btServerSocket?.close()
                        shouldLoop = false
                    }
                }
            }

            fun cancel() {
                try {
                    btServerSocket?.close()
                } catch (ioe: IOException) {
                    Log.e(TAG, "Couldn't close socket connection", ioe)
                }
            }
        }

    }

    private fun startCamera() {
        val cameraProviderFuture = ProcessCameraProvider.getInstance(this)

        cameraProviderFuture.addListener({
//            bind camera lifecycle to lifecycle owner
            val cameraProvider: ProcessCameraProvider = cameraProviderFuture.get()

//            make the preview
            val preview = Preview.Builder().build().also {
                it.setSurfaceProvider(viewBinding.viewFinder.surfaceProvider)
            }

            val recorder = Recorder.Builder().setQualitySelector(QualitySelector.from(Quality.HIGHEST)).build()
            videoCapture = VideoCapture.withOutput(recorder)

            val imageAnalyzer = ImageAnalysis.Builder().build()
                .also {
                    it.setAnalyzer(cameraExecutor, LuminosityAnalyzer { luma ->
//                        Log.d(TAG, "Average luminosity: $luma")
                    })
                }

            val cameraSelector = CameraSelector.DEFAULT_BACK_CAMERA

            try {
//                unbind use cases before rebinding use cases to the camera
                cameraProvider.unbindAll()
                cameraProvider.bindToLifecycle(
                    this, cameraSelector, preview, videoCapture, imageAnalyzer
                )
            } catch (exc: Exception) {
                Log.e(TAG, "Use case binding failed", exc)
            }
        }, ContextCompat.getMainExecutor(this))


    }

    private fun captureVideo() {

        val videoCapture = videoCapture ?: return

        viewBinding.videoCaptureButton.isEnabled = false

        val currentRecording = recording
        if (currentRecording != null) {
            currentRecording.stop()
            recording = null
            return
        }

//        create and start new recording
        val name = SimpleDateFormat(FILENAME_FORMAT, Locale.US).format(System.currentTimeMillis())

        val contentValues = ContentValues().apply {
            put(MediaStore.MediaColumns.DISPLAY_NAME, name)
            put(MediaStore.MediaColumns.MIME_TYPE, "video/mp4")
            if (Build.VERSION.SDK_INT > Build.VERSION_CODES.P) {
                put(MediaStore.Video.Media.RELATIVE_PATH, "Movies/Narkissa/VideoScans")
            }
        }

        val mediaStoreOutputOptions = MediaStoreOutputOptions
            .Builder(contentResolver, MediaStore.Video.Media.EXTERNAL_CONTENT_URI)
            .setContentValues(contentValues)
            .build()

        recording = videoCapture.output.prepareRecording(this, mediaStoreOutputOptions)
            .start(ContextCompat.getMainExecutor(this)) { recordEvent ->
                when(recordEvent) {
                    is VideoRecordEvent.Start -> {
                        viewBinding.videoCaptureButton.apply {
                            text = getString(R.string.stop_capture)
                            isEnabled = true
                        }
                    }
                    is VideoRecordEvent.Finalize -> {
                        if (!recordEvent.hasError()) {
                                val msg = "Video capture succeeded: " +
                                    "${recordEvent.outputResults.outputUri}"
                                Toast.makeText(this, msg, Toast.LENGTH_SHORT).show()
                                Log.d(TAG, msg)
                        } else {
                            recording?.close()
                            recording = null
                            Log.e(TAG, "Video capture ends with error: " + "${recordEvent.error}")
                        }
                        viewBinding.videoCaptureButton.apply {
                        text = getString(R.string.video_capture_text)
                        isEnabled = true
                        }
                    }
                }
            }
    }

    // 18-7-2024: trying to set up bluetooth receiver, type mismatches occurring
    private val receiver = object: BroadcastReceiver() {
        override fun onReceive(context: Context, intent: Intent) {
            val action: String? = intent.action
            when(action) {
                BluetoothDevice.ACTION_FOUND -> {
                    try {
                        val device: BluetoothDevice = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE)!!
                        val deviceName = device.name
                        val deviceMACAddress = device.address
                        Log.d(TAG, "Found device ${deviceMACAddress}")
                    } catch (se: SecurityException) {
                        Log.d(SECURITY_EXCEPTION, "${se}")
                    }
                }
            }
        }
    }

    private fun requestPermissions() {
        activityResultLauncher.launch(REQUIRED_PERMISSIONS)
    }

    private val activityResultLauncher =
        registerForActivityResult(
            ActivityResultContracts.RequestMultiplePermissions())
        { permissions ->
//          Loop through permissions and return permissionsGranted status
//          if perms are granted or not
            var permissionsGranted = true
            permissions.entries.forEach {
                if (it.key in REQUIRED_PERMISSIONS && it.value ==  false)
                    permissionsGranted = false
            }
            if (!permissionsGranted) {
                Toast.makeText(baseContext,
                    "Permission requests denied",
                    Toast.LENGTH_SHORT).show()
            } else {
                startCamera()
            }
        }

    private fun allPermissionsGranted() = REQUIRED_PERMISSIONS.all {
        ContextCompat.checkSelfPermission(
            baseContext, it) == PackageManager.PERMISSION_GRANTED
    }

    override fun onDestroy() {
        super.onDestroy()
        cameraExecutor.shutdown()
        unregisterReceiver(receiver)
    }

    companion object {
        private const val TAG = "Narkissa"
        private const val NAME = "Narkissa_Body"
        private val NB_UUID = UUID.fromString("dbd156b1-4c95-4d3d-8e3a-10e180b460e4")
        // NB_UUID generated by uuidgenerator.net/version4
        private const val SECURITY_EXCEPTION = "SecurityException"
        private const val FILENAME_FORMAT = "yyyy-MM-dd-HH-mm-ss-SSS"
        private val REQUIRED_PERMISSIONS =
            mutableListOf(
                Manifest.permission.CAMERA,
                Manifest.permission.BLUETOOTH,
                Manifest.permission.BLUETOOTH_SCAN,
                Manifest.permission.BLUETOOTH_ADMIN,
                Manifest.permission.BLUETOOTH_CONNECT
            ).apply {
                if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.P) {
                    add(Manifest.permission.WRITE_EXTERNAL_STORAGE)
                }
            }.toTypedArray()
    }

    private class LuminosityAnalyzer(private val listener: LumaListener) : ImageAnalysis.Analyzer {
        private fun ByteBuffer.toByteArray(): ByteArray {
            rewind()  // rewinds the buffer to zero
            val data = ByteArray(remaining())
            get(data) // copy buffer into a byte array
            return data
        }

        override fun analyze(image: ImageProxy) {
            val buffer = image.planes[0].buffer
            val data = buffer.toByteArray()
            val pixels = data.map { it.toInt() and 0xFF }
            val luma = pixels.average()

            listener(luma)

            image.close()
        }
    }
}