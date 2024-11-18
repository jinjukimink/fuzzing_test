#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libyang.h"

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  LY_ERR ret;
  struct ly_ctx *ctx = NULL;
  struct lyd_node *root = NULL, *op = NULL;
  struct ly_in *in = NULL;
  char *str = NULL;

  if (Size < 1) {
    return 0;
  }

  str = malloc(Size + 1);
  if (!str) {
    return 0;
  }

  memcpy(str, Data, Size);
  str[Size] = '\0';

  ret = ly_ctx_new(NULL, 0, &ctx);
  if (ret != LY_SUCCESS) {
    free(str);
    return 0;
  }

  ret = ly_in_new_memory(str, &in);
  if (ret != LY_SUCCESS) {
    free(str);
    ly_ctx_destroy(ctx);
    return 0;
  }

  // Parse the input data as an operation.
  ret = lyd_parse_op(ctx, NULL, in, LYD_XML, LYD_TYPE_DATA_YANG, &op, &root);
  // Parsing failed, but that's okay for fuzzing.

  lyd_free_all(root);
  lyd_free_all(op);
  ly_in_free(in, 0); // The original code was missing the second argument.
  ly_ctx_destroy(ctx);
  free(str);
  return 0;
}