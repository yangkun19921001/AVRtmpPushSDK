## AVRtmpPushSDK

欢迎关注 **AVRtmpPushSDK** 直播开源项目，这是一个由 kotlin 、C++ 编写的项目。整个项目完成了采集、 视音频处理、编码、数据发送前处理、数据发送的功能。整个项目支持 flv 封包，rtmp 上传，当然也向外面提供了封包和上传的相关接口。 整个项目均由个人编写完成，可能某些地方会有一些不足之处，欢迎您提出宝贵的意见和建议。



### 支持功能

>- 音视频硬件编码
>- 动态码率调整
>- 摄像头参数设置
>- 音频编码参数设置
>- 视频编码参数设置
>- 支持添加图片、文字水印
>- 支持摄像头切换
>- 支持 flv 封包
>- 支持自动对焦
>- 支持 Live 模式下横竖屏动态切换



###要求

>Android SDK 版本 18+



### 如何使用

#### 1、添加远程依赖

```groovy
implementation 'com.devyk.av.rtmp.library:AVRtmpPushSDK:1.0.0'
```

#### 2、添加权限

```
<uses-permission android:name="android.permission.CAMERA" />
<uses-permission android:name="android.permission.FLASHLIGHT" />
<uses-permission android:name="android.permission.CAMERA" />

<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"/>
<uses-permission android:name="android.permission.RECORD_AUDIO"/>
<uses-permission android:name="android.permission.MODIFY_AUDIO_SETTINGS" />

<uses-permission android:name="android.permission.WAKE_LOCK" />
<uses-permission android:name="android.permission.INTERNET" />

<uses-feature android:name="android.hardware.camera" />
<uses-feature android:name="android.hardware.autofocus" />
```

ps: 在Android 6.0以后很多权限需要动态申请. 如果你想快速使用，可以将项目的targetSdkVersion设置在23一下。

#### 3、预览窗口

```kotlin
    <com.devyk.av.rtmp.library.widget.AVLiveView
            android:id="@+id/live"
            android:layout_width="match_parent"
            android:layout_height="match_parent"
            live:back="true"
            live:fps="25"
            live:sampleRate="44100"
            live:preview_height="1280"
            live:preview_width="720"
            live:videoMaxRate="300"
            live:videoMinRate="200"
    />
```



#### 4、参数配置

**4.1、XML 配置**

>是否后置摄像头优先 -> live:back
>
>预览编码的帧率 -> live:fps
>
>音频采样率 ->  live:sampleRate
>
>预览宽高 ->  live:preview_height/live:preview_width
>
>视频最大码率 -> live:videoMaxRate
>
>视频最低码率 -> live:videoMinRate



**4.2、代码配置**

4.2.1 、音频采集编码参数设置

```kotlin
//初始化音频参数
var audioConfiguration = AudioConfiguration.createDefault()
live.setAudioConfigure(audioConfiguration)
```

4.2.2、视频编码参数设置

```kotlin
        var videoConfiguration = VideoConfiguration.Builder()
            .setBps(400, 800)
            .setFps(25)
            .setCodeType(VideoConfiguration.ICODEC.ENCODE)
            .setSize(320, 240)
            .setIfi(5)
            .setMediaCodec(true)
            .build()
        live.setVideoConfigure(videoConfiguration)
```

4.2.3、camera 预览参数设置

```kotlin
        //初始化 camera 参数
        var cameraConfiguration = CameraConfiguration.Builder()
            .setFacing(CameraConfiguration.Facing.FRONT)
            .setFps(25)
            .setPreview(320, 240)
            .build()
        live.setCameraConfigure(cameraConfiguration)
```



####5、设置打包器

```kotlin
var mPacker = RtmpPacker()
live.setPacker(mPacker)
```



#### 6、设置发送器

```kotlin
private var mDataSource = "rtmp://[ip]:[port]/devykLive/live1"

 mSender = RtmpSender()
 mSender.setDataSource(mDataSource)
 live.setSender(mSender)
```



#### 7、添加水印

```kotlin
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
     * 默认水印坐标地址，对应在屏幕右下角，用户可以自定义坐标
     */
    private var mVertexData = floatArrayOf( 
        0.55f, -0.9f, //第一个点 左下角
        0.9f, -0.9f, //第二个点 右下角
        0.55f, -0.7f, //第三个点 左上角
        0.9f, -0.7f //第四个点  右上角
    )

//设置 Bitmap 水印 第二个参数如果传 null 那么默认在右下角
live.setWatermark(Watermark(BitmapFactory.decodeResource(resources,R.mipmap.live_logo), mVertexData))

//设置文字水印 textSize 设置 20 那么会根据设置的坐标进行拉伸，传入 null 内部自动处理
live.setWatermark(Watermark("随播",Color.WHITE,20,null))
```



#### 8、开始预览

```kotlin
live.startPreview()
```



#### 9、连接 rtmp 服务器

```kotlin
mSender?.setOnConnectListener(lis: OnConnectListener)
mSender?.connect()
 
 //rtmp 直播服务器连接回调
 public interface OnConnectListener {
    /**
     * 开始链接
     */
    fun onConnecting()

    /**
     * 连接成功
     */
    fun onConnected()

    /**
     * 推送失败
     */
    fun onFail(message:String)

    /**
     * 关闭
     */
    fun onClose()
}
```

#### 10、开始直播

```kotlin
//开始进行打包
mPacker.start()
//开始进行直播推流
live.startLive()
```



#### 11、动态设置码率

````kotlin
 live.setVideoBps(bps:Int)
````



#### 12、停止直播

```kotlin
//停止音视频采集编解码
live.stopLive()
//关闭 rtmp 服务
mSender?.close()
//关闭打包器
mPacker.stop()
```



#### 13、横竖屏适配

```kotlin
//1、Activity 页面添加如下配置
   android:configChanges="orientation|keyboardHidden|screenSize"

//2. 实现横竖屏切换监听
    override fun onConfigurationChanged(newConfig: Configuration) {
        super.onConfigurationChanged(newConfig)
        live.previewAngle(this)
    }
```





 

