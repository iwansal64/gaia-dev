// Dependencies
#include <Arduino.h>
#include <APIManager.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <env.h>

const char *change_user_id_api_url = "https://gaia-odc.digital/api/connect/device";

APIResult APIManager::change_user_id(String user_id) {
  if(!WiFiManager::isConnected) return API_NoWIFI;

  HTTPClient client;

  client.begin(change_user_id_api_url);
  client.addHeader("Content-Type", "application/json");
  client.addHeader("Cookie", "access_token="+String(ENV_DEVICE_ACCESS_TOKEN));

  String request_body = "{ \"user_id\": \"" + user_id + "\" }";
  int repsonse_code = client.POST(request_body);

  Serial.printf("[API] Response Code: %d\n", repsonse_code);
  
  switch(repsonse_code) {
    case 200:
      return API_OK;
    case 401:
      return API_Unauthorized;
    case 409:
      return API_Conflict;
    case 404:
      return API_NotFound;
    default:
      return API_Error;
  }
}