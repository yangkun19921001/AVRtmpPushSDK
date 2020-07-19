package com.devyk.av.rtmp.library.widget

import android.content.Context
import android.util.AttributeSet
import com.devyk.av.rtmp.library.R
import com.devyk.av.rtmp.library.callback.ICameraOpenListener
import com.devyk.av.rtmp.library.camera.Watermark
import com.devyk.av.rtmp.library.config.AudioConfiguration
import com.devyk.av.rtmp.library.config.CameraConfiguration
import com.devyk.av.rtmp.library.config.VideoConfiguration
import com.devyk.av.rtmp.library.controller.StreamController
import com.devyk.av.rtmp.library.stream.packer.Packer
import com.devyk.av.rtmp.library.stream.sender.Sender

/**
 * <pre>
 *     author  : devyk on 2020-07-16 15:50
 *     blog    : https://juejin.im/user/578259398ac2470061f3a3fb/posts
 *     github  : https://github.com/yangkun19921001
 *     mailbox : yang1001yk@gmail.com
 *     desc    : This is LiveView
 * </pre>
 */
public class AVLiveView : CameraView, ICameraOpenListener {


    private var mFps = 20
    private var mPreviewWidth = 720
    private var mPreviewHeight = 1280
    private var mBack = true
    private var mSampleRate = 44100
    private var mVideoMinRate = 400
    private var mVideoMaxRate = 1800


    private var mVideoConfiguration=VideoConfiguration.createDefault()
    private var mAudioConfiguration=AudioConfiguration.createDefault()
    private var mCameraConfiguration=CameraConfiguration.createDefault()


    private var mStreamController: StreamController? = null

    constructor(context: Context?) : this(context, null)
    constructor(context: Context?, attrs: AttributeSet?) : this(context, attrs, 0)
    constructor(context: Context?, attrs: AttributeSet?, defStyleAttr: Int) : super(context, attrs, defStyleAttr) {
        init(context!!, attrs)
    }

    private fun init(context: Context, attrs: AttributeSet?) {
        var typeArray = context.obtainStyledAttributes(attrs, R.styleable.AVLiveView)
        mFps = typeArray.getInteger(R.styleable.AVLiveView_fps, mFps)
        mPreviewHeight = typeArray.getInteger(R.styleable.AVLiveView_preview_height, mPreviewHeight)
        mPreviewWidth = typeArray.getInteger(R.styleable.AVLiveView_preview_width, mPreviewWidth)
        mSampleRate = typeArray.getInteger(R.styleable.AVLiveView_sampleRate, mSampleRate)
        mBack = typeArray.getBoolean(R.styleable.AVLiveView_back, mBack)
        mVideoMinRate = typeArray.getInteger(R.styleable.AVLiveView_videoMinRate, mVideoMinRate)
        mVideoMaxRate = typeArray.getInteger(R.styleable.AVLiveView_videoMaxRate, mVideoMaxRate)

        //实例化数据流的控制器
        mStreamController = StreamController()
        // Camera 预览配置
       mCameraConfiguration = CameraConfiguration.Builder()
            .setFacing(if (mBack) CameraConfiguration.Facing.BACK else CameraConfiguration.Facing.FRONT)
            .setFps(mFps)
            .setPreview(mPreviewHeight, mPreviewWidth)
            .build()
        //配置音频参数
        mAudioConfiguration = AudioConfiguration.Builder().setFrequency(mSampleRate)
            .setAec(true)
            .setMediaCodec(true)
            .build()
        //视频编码参数配置
        mVideoConfiguration = VideoConfiguration.Builder()
            .setSize(mPreviewWidth, mPreviewHeight)
            .setBps(mVideoMinRate, mVideoMaxRate)
            .setFps(mFps)
            .setMediaCodec(true)
            .build()
        //添加 Camera 打开的监听
        addCameraOpenCallback(this)
    }

    /**
     * 设置音频编码和采集的参数
     */
    fun setAudioConfigure(audioConfiguration: AudioConfiguration) {
      this.mAudioConfiguration = audioConfiguration
    }

    /**
     * 设置视频编码参数
     */
    fun setVideoConfigure(videoConfiguration: VideoConfiguration){
        this.mVideoConfiguration = videoConfiguration
    }

    /**
     * 设置预览视频的参数
     */
    fun setCameraConfigure(cameraConfiguration: CameraConfiguration){
        this.mCameraConfiguration = cameraConfiguration

    }

    /**
     * 开始预览
     */
    fun startPreview(){
        mStreamController?.setAudioConfigure(mAudioConfiguration)
        mStreamController?.setVideoConfigure(mVideoConfiguration)
        //开始预览
        startPreview(mCameraConfiguration)
    }


    override fun setWatermark(watermark: Watermark) {
        super.setWatermark(watermark)
        mStreamController?.setWatermark(watermark)
    }





    /**
     * 设置打包器
     */
    fun setPacker(packer: Packer) {
        mStreamController?.setPacker(packer)
    }

    /**
     * 设置发送器
     */
    fun setSender(sender: Sender) {
        mStreamController?.setSender(sender)
    }

    /**
     * camera 打开可以初始化了
     */
    override fun onCameraOpen() {
        mStreamController?.prepare(context, getTextureId(), getEGLContext())
    }


    /**
     * 开始
     */
    fun startLive() {
        mStreamController?.start()

    }

    /**
     * 暂停
     */
    fun pause() {
        mStreamController?.pause()

    }

    /**
     * 恢复
     */
    fun resume() {
        mStreamController?.resume()

    }

    /**
     * 停止
     */
    fun stopLive() {
        mStreamController?.stop()

    }

    /**
     * 禁言
     */
    fun setMute(isMute: Boolean) {
        mStreamController?.setMute(isMute)
    }

    /**
     * 动态设置视频编码码率
     */
    fun setVideoBps(bps: Int) {
        mStreamController?.setVideoBps(bps)
    }



    /**
     * 释放相机
     */
    override fun releaseCamera() {
        super.releaseCamera()
        mStreamController = null
    }
}