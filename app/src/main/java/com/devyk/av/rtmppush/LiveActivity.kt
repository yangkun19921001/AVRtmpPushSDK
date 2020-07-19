package com.devyk.av.rtmppush

import android.content.res.Configuration
import android.graphics.BitmapFactory
import android.graphics.Color
import android.os.Build
import android.text.TextUtils
import android.view.View
import android.view.ViewGroup
import android.widget.Button
import android.widget.EditText
import android.widget.LinearLayout
import android.widget.Toast
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AlertDialog
import com.devyk.av.rtmp.library.callback.OnConnectListener
import com.devyk.av.rtmp.library.camera.Watermark
import com.devyk.av.rtmp.library.config.AudioConfiguration
import com.devyk.av.rtmp.library.config.CameraConfiguration
import com.devyk.av.rtmp.library.config.VideoConfiguration
import com.devyk.av.rtmp.library.stream.packer.DefaultPacker
import com.devyk.av.rtmp.library.stream.packer.rtmp.RtmpPacker
import com.devyk.av.rtmp.library.stream.sender.rtmp.RtmpSender
import com.devyk.av.rtmp.library.utils.LogHelper
import com.devyk.ikavedit.base.BaseActivity
import kotlinx.android.synthetic.main.activity_live.*

class LiveActivity : BaseActivity<Int>(), OnConnectListener {
    /**
     * OpenGL 物体坐标，对应 Android 屏幕坐标
     *
     * -1.0，1.0                             1.0，1.0
     *  -------------------------------------
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                                   |
     *  |                   这里就是水印坐标   |
     *  |                          |-----    |
     *  |                          |    |    |
     *  |                          ——-—-|    |
     *  --------------------------------------
     * -1.0，1.0                             1.0，-1.0
     */
    private var mVertexData = floatArrayOf(
        0.55f, -0.9f, //第一个点 左下角
        0.9f, -0.9f, //第二个点 右下角
        0.55f, -0.7f, //第三个点 左上角
        0.9f, -0.7f //第四个点  右上角
    )


    private var mDataSource = "rtmp://www.devyk.cn:1992/devykLive/live1"
    private var isConncet = false
    private lateinit var mSender: RtmpSender
    private lateinit var mPacker: RtmpPacker
    private var uploadDialog: AlertDialog? = null


    override fun initListener() {
        mSender?.setOnConnectListener(this)
    }

    override fun initData() {

        //设置文字水印
        setWatemark()
    }


    override fun init() {
        //初始化 RTMP 发送器
        mSender = RtmpSender()
        //初始化包封装器
        mPacker = RtmpPacker()
        live.setPacker(mPacker)
        live.setSender(mSender)

        //初始化音频参数
        var audioConfiguration = AudioConfiguration.createDefault()
        live.setAudioConfigure(audioConfiguration)

        //初始化视频编码参数
        var videoConfiguration = VideoConfiguration.Builder()
            .setBps(400, 800)
            .setFps(25)
            .setCodeType(VideoConfiguration.ICODEC.ENCODE)
            .setSize(320, 240)
            .setIfi(5)
            .setMediaCodec(true)
            .build()
        live.setVideoConfigure(videoConfiguration)

        //初始化 camera 参数
        var cameraConfiguration = CameraConfiguration.Builder()
            .setFacing(CameraConfiguration.Facing.FRONT)
            .setFps(25)
            .setPreview(320, 240)
            .build()
        live.setCameraConfigure(cameraConfiguration)

        //设置预览
        live.startPreview()

        mSender.setDataSource(mDataSource)

        initRtmpAddressDialog()

    }

    override fun onContentViewBefore() {
        super.onContentViewBefore()
        Utils.init(application)
        checkPermission()
        setNotTitleBar()
    }

    override fun getLayoutId(): Int = R.layout.activity_live


    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        LogHelper.e(TAG, "方向改变:${newConfig.densityDpi}")
        live.previewAngle(this)
    }


    public fun rtmp_live(view: View) {
        if (isConncet) {
            progressBar.visibility = View.VISIBLE;
            live.stopLive()
            mSender?.close()
            isConncet = false
            mPacker.stop()
            return
        }
        isConncet = !isConncet
        uploadDialog?.show()
    }

    public fun camera_change(view: View) {
        live.switchCamera()
    }


    override fun onDestroy() {
        super.onDestroy()
        mSender?.close()
        live.stopLive()
        live.releaseCamera()
    }


    override fun onFail(message: String) {
        runOnUiThread {
            Toast.makeText(applicationContext, message, Toast.LENGTH_LONG).show()
            progressBar.visibility = View.GONE;
            live_icon.setImageDrawable(getDrawable(R.mipmap.live))
        }
    }

    @RequiresApi(Build.VERSION_CODES.N)
    override fun onConnecting() {
        runOnUiThread {
            progressBar.visibility = View.VISIBLE;
        }
    }

    override fun onConnected() {
        mPacker.start()
        live.startLive()
        //传输的过程中可以动态设置码率
        live.setVideoBps(300)
        runOnUiThread {
            live_icon.setImageDrawable(getDrawable(R.mipmap.stop))
            progressBar.visibility = View.GONE;
        }
    }

    override fun onClose() {
        runOnUiThread {
            progressBar.visibility = View.GONE
            live_icon.setImageDrawable(getDrawable(R.mipmap.live))
        }
    }

    fun initRtmpAddressDialog() {
        val inflater = layoutInflater
        val playView = inflater.inflate(R.layout.address_dialog, findViewById<ViewGroup>(R.id.dialog))
        var address = playView.findViewById<EditText>(R.id.address)
        address.setText(mDataSource)
        val okBtn = playView.findViewById<Button>(R.id.ok)
        val cancelBtn = playView.findViewById<Button>(R.id.cancel)
        val uploadBuilder = AlertDialog.Builder(this)
        uploadBuilder.setTitle("输入推流地址")
        uploadBuilder.setView(playView)
        uploadDialog = uploadBuilder.create()
        okBtn.setOnClickListener {
            val uploadUrl = address.getText().toString()
            if (TextUtils.isEmpty(uploadUrl)) {
                Toast.makeText(applicationContext, "Upload address is empty!", Toast.LENGTH_SHORT).show()
            } else {
                //设置 rtmp 地址
                mSender.setDataSource(uploadUrl)
                //开始连接
                mSender.connect()
            }
            uploadDialog?.dismiss()
        }
        cancelBtn.setOnClickListener { uploadDialog?.dismiss() }
    }


    private fun setWatemark() {
        //设置 Bitmap 水印 第二个参数如果传 null 那么默认在右下角
//        live.setWatermark(Watermark(BitmapFactory.decodeResource(resources, R.mipmap.live_logo), null))
        live.setWatermark(Watermark("DevYK", Color.WHITE, 20, null))
    }
}
