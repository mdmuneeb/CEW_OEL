#include <stdio.h>
#include <curl/curl.h>
#include <jansson.h>

// Function to handle the API response
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    
    // Parse the JSON response and extract the wind speed
    json_error_t error;
    json_t *root = json_loadb(contents, size * nmemb, 0, &error);

    if (root != NULL) {

        // Appending the raw data to the Weather_Data.txt file
        FILE *file = fopen("Weather_Data.txt", "a");

        if (file != NULL) {
            fwrite(contents, size, nmemb, file);
            fclose(file);
        }

        // Checking if "wind" object exists in the JSON response
        json_t *windObject = json_object_get(root, "wind");

        if (windObject != NULL && json_is_object(windObject)) {
            // Extracted wind speed
            json_t *speed = json_object_get(windObject, "speed");
            double speedV = json_number_value(speed);


            if (json_number_value(speed) > 1.0 || json_number_value(speed) < 0)
            {
                FILE *file;

                file = fopen("Anomaly.log", "a"); 
                
                if (file != NULL)
                {
                    char email_command[1024];
                    fprintf(file, "Extreme windspeed %.2f recorded at  Date: %s, Time: %s  \n", speedV, __DATE__, __TIME__);
                    snprintf(email_command, sizeof(email_command),
                            "echo -e 'From: muneebzafar999@gmail.com\nTo: muhammadmuneebzafar5@gmail.com\nSubject: High Wind Speed Alert\n\nWind Speed is above threshold: %.2f(km/h)' | "
                            "curl --ssl-reqd --url 'smtps://smtp.gmail.com:465' "
                            "-u muneebzafar999@gmail.com:mhkuheurwstpghml " // Fixed authentication credentials
                            "--mail-from 'muneebzafar999@gmail.com' "
                            "--mail-rcpt 'muhammadmuneebzafar5@gmail.com' "
                            "--upload-file -", // Remove --mail-auth for OAuth 2.0
                            speedV);
                    system(email_command);

                }
            }


            if (speed != NULL && json_is_number(speed)) {
                double windSpeed = json_number_value(speed);

                // Appending the wind speed to the Wind_Speed_Data.txt file
                file = fopen("Wind_Speed_Data.txt", "a");

                if (file != NULL) {
                    fprintf(file, "%.2f\n", windSpeed);
                    fclose(file);
                }
            }
        }

        json_decref(root);
    }

    return size * nmemb;
}

int main() {
    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Initialize a libcurl easy handle
    CURL *curl = curl_easy_init();

    // Set the API URL
    const char *url = "https://api.openweathermap.org/data/2.5/weather?units=metric&q=karachi&appid=7821014de233ecf098180185706f59ca";

    if (curl) {
        // Set the URL to fetch
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the callback function to handle the API response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

        // Perform the request
        CURLcode res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        // Clean up
        curl_easy_cleanup(curl);
    }

    // Cleanup libcurl
    curl_global_cleanup();

    return 0;
}

