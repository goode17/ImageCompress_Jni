package com.goode.imgcompress.reduce.data;

import android.graphics.Bitmap;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.support.annotation.CheckResult;
import android.support.annotation.DrawableRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.RawRes;

import java.io.File;
import java.io.IOException;
import java.net.URL;

/**
 * Ensures that the set of explicitly supported model types remains consistent across Glide's
 * API surface.
 */
interface ModelTypes<T> {
  @NonNull
  @CheckResult
  T source(@Nullable Bitmap bitmap);

  @NonNull
  @CheckResult
  T source(@Nullable Bitmap[] bitmaps);

  @NonNull
  @CheckResult
  T source(@Nullable Drawable drawable);

  @NonNull
  @CheckResult
  T source(@Nullable String string);

  @NonNull
  @CheckResult
  T source(@Nullable Uri uri) throws IOException;

  @NonNull
  @CheckResult
  T source(@Nullable File file);

  @NonNull
  @CheckResult
  T source(@Nullable File[] files);

  @NonNull
  @CheckResult
  T source(@RawRes @DrawableRes @Nullable Integer resourceId);

  @Deprecated
  @CheckResult
  T source(@Nullable URL url) throws IOException;

  @NonNull
  @CheckResult
  T source(@Nullable byte[] model);

}
