#include <stdlib.h>
#include <string.h>

// copyright: ChatGPT :)
void expand_env_vars(const char *src, char *dest) {
    int i = 0, j = 0;
    while (src[i] != '\0') {
        if (src[i] == '$') {
            char *env_name = (char*)malloc(sizeof(char) * 128);
            int k = 0;
            i++;
            if (src[i] == '{') {
                i++;
                while (src[i] != '}' && src[i] != '\0') {
                    env_name[k] = src[i];
                    i++; k++;
                }
                if (src[i] == '}') {
                    i++;
                } else {
                    env_name[k] = '\0';
                    dest[j++] = '$';
                    strncpy(&dest[j], env_name, k);
                    j += k;
                    free(env_name);
                    continue;
                }
            } else if (src[i] >= 'A' && src[i] <= 'Z') {
                while ((src[i] >= 'A' && src[i] <= 'Z') || (src[i] >= '0' && src[i] <= '9') || src[i] == '_') {
                    env_name[k] = src[i];
                    i++; k++;
                }
            } else {
                env_name[k] = '\0';
                dest[j++] = '$';
                strncpy(&dest[j], env_name, k);
                j += k;
                free(env_name);
                continue;
            }
            env_name[k] = '\0';
            char *env_value = getenv(env_name);
            free(env_name);
            if (env_value != NULL) {
                int len = strlen(env_value);
                strncpy(&dest[j], env_value, len);
                j += len;
            }
        } else {
            dest[j++] = src[i++];
        }
    }
    dest[j] = '\0';
}
