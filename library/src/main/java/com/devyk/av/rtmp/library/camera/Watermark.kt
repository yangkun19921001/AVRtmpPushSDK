package com.devyk.av.rtmp.library.camera

import android.graphics.Bitmap

/**
 * <pre>
 *     author  : devyk on 2020-07-18 19:43
 *     blog    : https://juejin.im/user/578259398ac2470061f3a3fb/posts
 *     github  : https://github.com/yangkun19921001
 *     mailbox : yang1001yk@gmail.com
 *     desc    : This is Watermark
 * </pre>
 */
class Watermark {

    var markImg: Bitmap? = null
    var txt: String? = null
    var textColor = -1
    var textSize = -1

    var floatArray: FloatArray? = null


    constructor(
        markImg: Bitmap
        , floatArray: FloatArray?
    ) {
        this.markImg = markImg
        this.floatArray = floatArray
    }

    constructor(
        txt: String,
        txtColor: Int,
        txtSize: Int
        , floatArray: FloatArray?
    ) {
        this.txt = txt
        this.textSize = txtSize
        this.textColor = txtColor
        this.floatArray = floatArray
    }

}