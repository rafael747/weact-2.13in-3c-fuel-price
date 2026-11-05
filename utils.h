#ifndef MAX_STATIONS
#define MAX_STATIONS 3
#endif

struct Station {
  Station() { clear(); }

  void clear() {
    set_name("XXXX");
    this->e5_price = 0;
    this->e10_price = 0;
    this->diesel_price = 0;
  }

  void set_name(const char *brand) {
    if (brand == nullptr) {
      return;
    }
    strncpy(this->name, brand, sizeof(this->name));
    this->name[sizeof(this->name) - 1] = '\0';
  }

  char name[13];
  float e5_price;
  float e10_price;
  float diesel_price;
};

Station bestStation;

void parseStationData(JsonObject root) {
  bestStation.clear();
  // Sentinel so the first valid diesel price wins.
  bestStation.diesel_price = 99;

  if (!root["stations"].is<JsonArray>()) {
    ESP_LOGW("parseStationData", "no 'stations' array in response");
    bestStation.clear();
    return;
  }

  int i = 0;
  JsonArray station_list = root["stations"].as<JsonArray>();
  for (JsonObject station : station_list) {
    Station currentStation;
    currentStation.set_name(station["brand"].as<const char *>());

    JsonArray fuels = station["fuels"].as<JsonArray>();
    for (JsonObject fuel : fuels) {
      if (!fuel["name"].is<const char *>() || !fuel["price"].is<float>()) {
        continue;
      }
      const char *name = fuel["name"];
      const float price = fuel["price"];

      if (strcmp(name, "Diesel") == 0) {
        currentStation.diesel_price = price;
      } else if (strcmp(name, "Super E5") == 0) {
        currentStation.e5_price = price;
      } else if (strcmp(name, "Super E10") == 0) {
        currentStation.e10_price = price;
      }
    }

    // Pick cheapest diesel only after all fuels for this station are parsed.
    if (currentStation.diesel_price < bestStation.diesel_price) {
      bestStation = currentStation;
    }

    i++;
    if (i >= MAX_STATIONS) {
      break;
    }
  }

  // No valid station found — keep display-friendly defaults.
  if (bestStation.diesel_price >= 99) {
    bestStation.clear();
  }
}
