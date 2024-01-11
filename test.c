#include <stdio.h>
#include <stdlib.h>

int main() {
    char email_command[1024];
    float speed = 42.0; // replace with your actual wind speed value

    snprintf(email_command, sizeof(email_command),
             "echo -e 'From: muneebzafar999@gmail.com\nTo: muhammadmuneebzafar5@gmail.com\nSubject: High Wind Speed Alert\n\nWind Speed is above threshold: %.2f(km/h)' | "
             "curl --ssl-reqd --url 'smtps://smtp.gmail.com:465' "
             "-u muneebzafar999@gmail.com:mhkuheurwstpghml " // Fixed authentication credentials
             "--mail-from 'muneebzafar999@gmail.com' "
             "--mail-rcpt 'muhammadmuneebzafar5@gmail.com' "
             "--upload-file -", // Remove --mail-auth for OAuth 2.0
             speed);
    system(email_command);

    return 0;
}
