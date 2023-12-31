#include <stdio.h>
#include <curl/curl.h>

int main() {
    CURL *curl;
    CURLcode result;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();


    char url[] = "https://api.openweathermap.org/data/2.5/weather?units=metric&q=portugal&appid=7821014de233ecf098180185706f59ca";

    FILE *file;
    

    if (curl) {
        // Set the URL
        curl_easy_setopt(curl, CURLOPT_URL, url);

        file = fopen("Weather_Data.txt", "w");

        if (file == NULL)
        {
            printf("There is some error");
        }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, fwrite);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

        // Perform the request
        result = curl_easy_perform(curl);

        // Check for errors
        if (result != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(result));

 
        // Cleanup
        curl_easy_cleanup(curl);

        fclose(file);
    }



    // Cleanup libcurl
    curl_global_cleanup();

    return 0;
}
