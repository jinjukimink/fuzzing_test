#include <stdint.h>
#include <stddef.h>
#include "roaring/roaring.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  if (Size < 8) {
    return 0;
  }

  const uint32_t Value1 = *(const uint32_t *)Data;
  Data += 4;
  Size -= 4;

  const uint32_t Value2 = *(const uint32_t *)Data;
  Data += 4;
  Size -= 4;

  roaring_bitmap_t *Bitmap1 = roaring_bitmap_create();
  if (!Bitmap1) {
    return 0;
  }
  roaring_bitmap_add(Bitmap1, Value1);

  roaring_bitmap_t *Bitmap2 = roaring_bitmap_create();
  if (!Bitmap2) {
    roaring_bitmap_free(Bitmap1);
    return 0;
  }
  roaring_bitmap_add(Bitmap2, Value2);

  // Call the function to fuzz
  const _Bool Eager = (Value1 > Value2);
  roaring_bitmap_t *BitmapOut =
      roaring_bitmap_lazy_or(Bitmap1, Bitmap2, Eager);
  roaring_bitmap_free(BitmapOut);

  roaring_bitmap_free(Bitmap1);
  roaring_bitmap_free(Bitmap2);

  return 0;
}