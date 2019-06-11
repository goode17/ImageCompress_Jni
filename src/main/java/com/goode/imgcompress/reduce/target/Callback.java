package com.goode.imgcompress.reduce.target;

import com.goode.imgcompress.reduce.data.CallableResult;

/**
 * 压缩回调接口
 * <p>
 * {@link CallableResult}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public interface Callback {
    /**
     * 回调方法
     *
     * @param result
     */
    void callback(CallableResult result);
}
