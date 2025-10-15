#include <LocalStorage.h>
#include <Preferences.h>

Preferences preferences;

bool LocalStorage::isOpened = false;




void LocalStorage::open() {
  if(LocalStorage::isOpened) return;

  preferences.begin("gaia-storage", false); // Open the storage

  LocalStorage::isOpened = true;
}


void LocalStorage::close() {
  if(!LocalStorage::isOpened) return;
  
  preferences.end(); // Close the storage
  
  LocalStorage::isOpened = false;
}



String LocalStorage::grab(const char *key) {
  return preferences.getString(key); // Get the string
}



void LocalStorage::put(const char *key, String value) {
  preferences.putString(key, value);
}



void LocalStorage::load_wifi_creds(String &ssid_container, String &pass_container) {
  LocalStorage::open();                         // Open the storage
  ssid_container = LocalStorage::grab("ssid");  // Grab WiFi SSID
  pass_container = LocalStorage::grab("pass");  // Grab WiFi password
  LocalStorage::close();                        // Close the storage
}

void LocalStorage::save_wifi_creds(String new_ssid, String new_pass) {
  LocalStorage::open();                         // Open the storage
  LocalStorage::put("ssid", new_ssid);          // Put WiFi SSID to the Storage
  LocalStorage::put("pass", new_pass);          // Put WiFi password to the Storage
  LocalStorage::close();                        // Close the storage
}