#include <libusb-1.0/libusb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VENDOR_ID 0x04d9   // Holtek Semiconductor
#define PRODUCT_ID 0xa292  // ANNE PRO 2
#define INTERFACE 1
#define ENDPOINT_OUT 0x3

#define BREATHING 0x03
#define BREATHING_KEY 0x07
#define BREATHING_TRIGGER 0x0a
#define GRADUAL_BREATHING 0x04
#define POPTANG 0x05
#define RAINBOW 0x06
#define RAINBOW_TRIGGER 0x0b

int set_static(const char *channels[]);
int set_effect(unsigned char effect, const char *colors[]);
int write_data(unsigned char *data, int length);

int main(int argc, char const *argv[]) {
  int status = 1;

  if (argc == 5) {
    if (strcmp("static", argv[1]) == 0) {
      status = set_static(argv + 2);
    }
  } else if (argc == 14) {
    if (strcmp("breathing", argv[1]) == 0) {
      status = set_effect(BREATHING, argv + 2);
    } else if (strcmp("breathing-key", argv[1]) == 0) {
      status = set_effect(BREATHING_KEY, argv + 2);
    } else if (strcmp("breathing-trigger", argv[1]) == 0) {
      status = set_effect(BREATHING_TRIGGER, argv + 2);
    } else if (strcmp("gradual-breathing", argv[1]) == 0) {
      status = set_effect(GRADUAL_BREATHING, argv + 2);
    } else if (strcmp("poptang", argv[1]) == 0) {
      status = set_effect(POPTANG, argv + 2);
    } else if (strcmp("rainbow", argv[1]) == 0) {
      status = set_effect(RAINBOW, argv + 2);
    } else if (strcmp("rainbow-trigger", argv[1]) == 0) {
      status = set_effect(RAINBOW_TRIGGER, argv + 2);
    }
  }

  if (status > 0) {
    fprintf(stderr, "Usage: %s effect color...\n", argv[0]);
  }

  return status;
}

int set_static(const char *channels[]) {
  unsigned char data[15] = {0x7b, 0x10, 0x41, 0x10, 0x07, 0x00, 0x00, 0x7d,
                            0x20, 0x03, 0xff, 0x01, 0xff, 0xff, 0xff};

  data[12] = strtoul(channels[0], NULL, 10);
  data[13] = strtoul(channels[1], NULL, 10);
  data[14] = strtoul(channels[2], NULL, 10);

  return write_data(data, 15);
}

int set_effect(unsigned char effect, const char *colors[]) {
  unsigned char data[] = {
      0x7b, 0x10, 0x41, 0x10, 0x28, 0x00, 0x00, 0x7d, 0x20, 0x03, 0xff, 0x06,
      0xff, 0x00, 0x00, 0xff, 0x80, 0x00, 0xff, 0xff, 0x00, 0x80, 0xff, 0x00,
      0x00, 0xff, 0x00, 0x00, 0xff, 0x80, 0x00, 0xff, 0xff, 0x00, 0x80, 0xff,
      0x00, 0x00, 0xff, 0x80, 0x00, 0xff, 0xff, 0x00, 0xff, 0xff, 0x00, 0x80};

  data[11] = effect;

  size_t pos = 12;
  for (size_t i = 0; i < 12; i++) {
    int r, g, b;
    sscanf(colors[i], "%02x%02x%02x", &r, &g, &b);

    data[pos + i] = r;
    data[pos + i + 1] = g;
    data[pos + i + 2] = b;

    pos += 2;
  }

  return write_data(data, 48);
}

int write_data(unsigned char *data, int length) {
  struct libusb_context *ctx = NULL;
  struct libusb_device_handle *dev_handle = NULL;
  int status = 0;
  int res;

  res = libusb_init(&ctx);
  if (res != LIBUSB_SUCCESS) {
    fprintf(stderr, "Failed to init libusb: %d\n", res);
    return -2;
  }

  dev_handle = libusb_open_device_with_vid_pid(ctx, VENDOR_ID, PRODUCT_ID);
  if (dev_handle == NULL) {
    fprintf(stderr, "Could not find a known device\n");
    status = -3;
    goto exit;
  }

  libusb_set_auto_detach_kernel_driver(dev_handle, 1);

  res = libusb_claim_interface(dev_handle, INTERFACE);
  if (res < 0) {
    fprintf(stderr, "Could not claim desired interface for the device\n");
    status = -4;
    goto close;
  }

  res = libusb_interrupt_transfer(dev_handle, ENDPOINT_OUT, data, length, NULL,
                                  0);
  if (res < 0) {
    fprintf(stderr, "Could not write data to device\n");
    status = -5;
    goto release;
  }

release:
  libusb_release_interface(dev_handle, INTERFACE);

close:
  libusb_close(dev_handle);

exit:
  libusb_exit(ctx);

  return status;
}
