package com.goode.imgcompress.reduce;

import android.content.Context;
import android.content.ContextWrapper;
import android.support.annotation.NonNull;

import com.goode.imgcompress.reduce.data.SourceBuilder;

/**
 * A singleton to present a simple static interface for building compress task
 *
 * @author fengyu.li
 * @date 2018/4/12
 */
public class Reduce {

    public static SourceBuilder with(@NonNull Context context) {
        if (context == null) {
            throw new IllegalArgumentException("You can not start a load on a null Context");
        } else {
            if (context instanceof ContextWrapper) {
                return new SourceBuilder().setContext(context);
            } else {
                throw new IllegalArgumentException("Error context type,You can load a Activity,Fragment or AppContext");
            }
        }
    }

}
