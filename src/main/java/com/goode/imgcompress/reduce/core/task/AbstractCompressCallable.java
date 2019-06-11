package com.goode.imgcompress.reduce.core.task;

import com.goode.imgcompress.reduce.data.CallableResult;

import java.util.concurrent.Callable;

/**
 * 压缩任务抽象类
 * <p>
 * 实现{@link Callable}接口
 * {@link CallableResult} the result type of method {@code call}
 * </p>
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
abstract class AbstractCompressCallable implements Callable<CallableResult> {

}
