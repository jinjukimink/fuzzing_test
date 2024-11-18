#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "roaring/roaring.h"

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  if (size < 8) {
    return 0;
  }

  uint32_t first_len = *(uint32_t *)data;
  data += 4;
  size -= 4;
  uint32_t second_len = *(uint32_t *)data;
  data += 4;
  size -= 4;

  if (size < first_len + second_len) {
    return 0;
  }

  roaring_bitmap_t *first = roaring_bitmap_create();
  for (uint32_t i = 0; i < first_len; i++) {
    if (data[i] < 128) {
      roaring_bitmap_add(first, i * 100);
    }
  }
  roaring_bitmap_run_optimize(first);

  roaring_bitmap_t *second = roaring_bitmap_create();
  for (uint32_t i = 0; i < second_len; i++) {
    if (data[i + first_len] < 128) {
      roaring_bitmap_add(second, i * 100);
    }
  }
  roaring_bitmap_run_optimize(second);

  roaring_bitmap_t *result = roaring_bitmap_lazy_or(first, second, false);
  (void)result;
  roaring_bitmap_free(result);

  result = roaring_bitmap_lazy_or(first, second, true);
  roaring_bitmap_free(result);

  roaring_bitmap_free(first);
  roaring_bitmap_free(second);

  return 0;
}