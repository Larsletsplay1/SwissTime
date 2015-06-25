#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;

static void main_window_load(Window *window) {
  // Create Layer
  s_time_layer = text_layer_create(GRect(0, 55, 144, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_text(s_time_layer, "00");
  
  // Improve
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  
  //add child
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void main_window_unload(Window *window) {
  //Destroy Layer
  text_layer_destroy(s_time_layer);
}

static void update_time() {
  // Get tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  
  // Create long-lived
  //static char buffer[] = "00:00";
  
  // Get current hours and minutes
  //int hours = tick_time->tm_hour;
  //int minutes = tick_time->tm_min;
  
  // Generate Time
  static char strMinutes[] = "00";
  //char strHours[] = "00";
  strftime(strMinutes, sizeof("00"), "%M", tick_time);
  //strftime(strHours, sizeof("00"), "%H", tick_time);
  
  //Log Hours
  APP_LOG(APP_LOG_LEVEL_DEBUG, "minutes: %s", strMinutes);
  
  // Display Time
  text_layer_set_text(s_time_layer, "Wait...");
  text_layer_set_text(s_time_layer, strMinutes);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}
  
static void init() {
  // Create main Window
  s_main_window = window_create();
  
  // Set handlers
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  
  // Show window
  window_stack_push(s_main_window, true);
  
  // Register TickService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
  
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
