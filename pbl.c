#include <stdio.h>
#include <string.h>
#include <ctype.h>

// Convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
}

// Check if keyword exists in text
int contains(char text[], const char keyword[]) {
    return strstr(text, keyword) != NULL;
}

int main() {
    char post[500];

    printf("Enter post description:\n");
    fgets(post, sizeof(post), stdin);

    toLowerCase(post);

    if (contains(post, "#fashion") || contains(post, "clothes") || contains(post, "dress")) {
        printf("Category: FASHION & ACCESSORIES\n");
    }
    else if (contains(post, "#electronics") || contains(post, "mobile") || contains(post, "laptop")) {
        printf("Category: ELECTRONICS\n");
    }
    else if (contains(post, "#ai") || contains(post, "artificial intelligence") || contains(post, "machine learning")) {
        printf("Category: AI & TECHNOLOGY\n");
    }
    else if (contains(post, "#sports") || contains(post, "football") || contains(post, "cricket")) {
        printf("Category: SPORTS\n");
    }
    else {
        printf("Category: OTHERS\n");
    }

    return 0;
}