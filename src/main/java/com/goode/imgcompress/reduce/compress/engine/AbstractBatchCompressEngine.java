package com.goode.imgcompress.reduce.compress.engine;

import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

/**
 * 批量压缩引擎的抽象类
 *
 * @author fengyu.li
 * @date 2018/4/23
 */
abstract class AbstractBatchCompressEngine extends AbstractCompressEngine {
    int successCount = 0;
    int failCount = 0;
    int totalCount = 0;
    private Lock lock = new ReentrantLock();
    /**
     * Save some img paths to{@link com.goode.imgcompress.reduce.data.CallableResult#path}
     * Use a "," to split 1 and more paths
     */
    StringBuffer paths = new StringBuffer();

    void increaseCount() {
        lock.lock();
        try {
            successCount++;
        } finally {
            lock.unlock();
        }
    }

    void decreaseCount() {
        lock.lock();
        try {
            failCount++;
        } finally {
            lock.unlock();
        }
    }

    void totalCount() {
        lock.lock();
        try {
            totalCount++;
        } finally {
            lock.unlock();
        }
    }

    void appendPath(String path) {
        lock.lock();
        try {
            paths.append(path).append(",");
        } finally {
            lock.unlock();
        }
    }
}
