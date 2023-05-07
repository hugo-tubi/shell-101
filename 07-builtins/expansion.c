#include <stdlib.h>
#include <string.h>

void expand_env_vars(const char *src, char *dest) {
    int i = 0, j = 0;
    while (src[i] != '\0') {
        if (src[i] == '$' && src[i+1] >= 'A' && src[i+1] <= 'Z') {
            char *env_name = (char*)malloc(sizeof(char) * 128);
            int k = 0;
            i++;
            while (src[i] != ' ' && src[i] != '\0') {
                env_name[k] = src[i];
                i++; k++;
            }
            env_name[k] = '\0';
            char *env_value = getenv(env_name);
            free(env_name);
            if (env_value != NULL) {
                int len = strlen(env_value);
                strncpy(&dest[j], env_value, len);
                j += len;
            }
        }
        else {
            dest[j] = src[i];
            i++; j++;
        }
    }
    dest[j] = '\0';
}
