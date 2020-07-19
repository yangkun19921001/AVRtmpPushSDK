package com.devyk.av.rtmp.library.controller

import android.content.Context
import android.media.MediaCodec
import android.media.MediaFormat
import com.devyk.av.rtmp.library.callback.IController
import com.devyk.av.rtmp.library.camera.Watermark
import com.devyk.av.rtmp.library.config.AudioConfiguration
import com.devyk.av.rtmp.library.config.CameraConfiguration
import com.devyk.av.rtmp.library.config.VideoConfiguration
import com.devyk.av.rtmp.library.stream.PacketType
import com.devyk.av.rtmp.library.stream.packer.DefaultPacker
import com.devyk.av.rtmp.library.stream.packer.Packer
import com.devyk.av.rtmp.library.stream.sender.Sender
import com.devyk.av.rtmp.library.utils.LogHelper
import java.nio.ByteBuffer
import javax.microedition.khronos.egl.EGLContext

/**
 * <pre>
 *     author  : devyk on 2020-07-15 22:05
 *     blog    : https://juejin.im/user/578259398ac2470061f3a3fb/posts
 *     github  : https://github.com/yangkun19921001
 *     mailbox : yang1001yk@gmail.com
 *     desc    : This is StreamController
 * </pre>
 */
public class StreamController : IController.OnAudioDataListener, IController.OnVideoDataListener,
    Packer.OnPacketListener {


    private var TAG = javaClass.simpleName

    /**
     * 水印
     */
    private var mWatermark: Watermark? = null

    /**
     * 音频数据的管理
     */
    private var mAudioController: IController? = null
    /**
     * 视频数据的管理
     */
    private var mVideoController: VideoController? = null
    /**
     * 音频采集编码默认配置
     */
    private var mAudioConfiguration = AudioConfiguration.createDefault()
    /**
     * 视频编码默认配置
     */
    private var mVideoConfiguration = VideoConfiguration.createDefault()
    /**
     * 打包器
     */
    private var mPacker: Packer? = null
    /**
     * 发送器
     */
    private var mSender: Sender? = null

    private var mContext: Context? = null
    private var mTextureId = 0
    private var mEGLContext: EGLContext? = null


    /**
     * 设置音频编码和采集的参数
     */
    fun setAudioConfigure(audioConfiguration: AudioConfiguration) {
        this.mAudioConfiguration = audioConfiguration
    }

    /**
     * 设置视频的编码参数
     */
    fun setVideoConfigure(videoConfiguration: VideoConfiguration) {
        this.mVideoConfiguration = videoConfiguration
    }


    /**
     * 设置打包器
     */
    fun setPacker(packer: Packer) {
        this.mPacker = packer
    }

    /**
     * 设置发送器
     */
    fun setSender(sender: Sender) {
        this.mSender = sender
    }


    /**
     *  @see start 之前必须调用 prepare
     */
    fun prepare(context: Context?, textureId: Int, eglContext: EGLContext?) {
        this.mContext = context?.applicationContext
        this.mTextureId = textureId
        this.mEGLContext = eglContext
        init()
    }

    private fun init(

    ) {
        mContext?.let { context ->
            mAudioController = AudioController(mAudioConfiguration)
            mVideoController = VideoController(context, mTextureId, mEGLContext, mVideoConfiguration)
            mPacker?.setPacketListener(this)
            mAudioController?.setAudioDataListener(this)
            mVideoController?.setVideoDataListener(this)
            mWatermark?.let { watermark ->
                mVideoController?.setWatermark(watermark)
            }
        }
    }

    fun start() {
        if (mAudioController == null || mVideoController == null)
            init()
        mAudioController?.start()
        mVideoController?.start()

    }

    fun pause() {
        mAudioController?.pause()
        mVideoController?.pause()

    }

    fun resume() {
        mAudioController?.resume()
        mVideoController?.resume()
    }

    fun stop() {
        mAudioController?.stop()
        mVideoController?.stop()
        mAudioController = null
        mVideoController = null
    }

    fun setMute(isMute: Boolean) {
        mAudioController?.setMute(isMute)
    }

    fun setVideoBps(bps: Int) {
        mVideoController?.setVideoBps(bps)
    }

    override fun onError(error: String?) {
        LogHelper.e(TAG, error)
    }

    /**
     * 音频编码之后的数据交于打包器处理
     */
    override fun onAudioData(bb: ByteBuffer, bi: MediaCodec.BufferInfo) {
        mPacker?.onAudioData(bb, bi)
    }


    /**
     * 音频输出格式
     */
    override fun onAudioOutformat(outputFormat: MediaFormat?) {
    }

    /**
     * 视频输出格式
     */
    override fun onVideoOutformat(outputFormat: MediaFormat?) {
        val spsb = outputFormat?.getByteBuffer("csd-0")
        var sps = ByteArray(spsb!!.remaining())
        spsb!!.get(sps, 0, sps.size)
        val ppsb = outputFormat.getByteBuffer("csd-1")
        var pps = ByteArray(ppsb!!.remaining())
        ppsb!!.get(pps, 0, pps.size)
        mPacker?.onVideoSpsPpsData(sps, pps, PacketType.SPS_PPS)
    }

    /**
     * 视频编码数据交于打包
     */
    override fun onVideoData(bb: ByteBuffer?, bi: MediaCodec.BufferInfo?) {
        mPacker?.onVideoData(bb, bi)
    }


    /**
     * 打包完成的数据，准备发送
     */
    override fun onPacket(byteArray: ByteArray, packetType: PacketType) {
        mSender?.onData(byteArray, packetType)
    }

    override fun onPacket(sps: ByteArray?, pps: ByteArray?, packetType: PacketType) {
        mSender?.onData(sps!!, pps!!, packetType)
    }

    fun setWatermark(watermark: Watermark) {
        mWatermark = watermark

    }


}