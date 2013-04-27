#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x4B, 0xB6, 0xAE, 0xEA, 0x3D, 0xC8, 0x46, 0xA7, 0xA6, 0x24, 0x46, 0x9B, 0x50, 0xC3, 0x91, 0x91 }
PBL_APP_INFO(MY_UUID,
             "Pip-Boy 2000", "Marcus Kida",
             1, 0, /* App version */
             RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_WATCH_FACE);

Window window;
BmpContainer background_image;
TextLayer timeLayer;

void handle_second_tick(AppContextRef ctx, PebbleTickEvent *t) {

  (void)t;
  (void)ctx;

  static char timeText[] = "00:00:00"; // Needs to be static because it's used by the system later.

  PblTm currentTime;

  get_time(&currentTime);
  string_format_time(timeText, sizeof(timeText), "%T", &currentTime);
  text_layer_set_text(&timeLayer, timeText);

}

void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "Window Name");
  window_stack_push(&window, true /* Animated */);

  // This seems to be explicitely done, without it my background did not show up...
  resource_init_current_app(&APP_RESOURCES);

  // Initialize and add Background to current layer
  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image);
  layer_add_child(&window.layer, &background_image.layer.layer);

  // Add Time layer and compute tick
  text_layer_init(&timeLayer, GRect(5, 2, 100, 110));
  text_layer_set_text_color(&timeLayer, GColorBlack);
  text_layer_set_background_color(&timeLayer, GColorClear);
  text_layer_set_font(&timeLayer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  handle_second_tick(ctx, NULL);

  layer_add_child(&window.layer, &timeLayer.layer);
}

void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image);
}

void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,
    
    .tick_info = {
      .tick_handler = &handle_second_tick,
      .tick_units = SECOND_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
