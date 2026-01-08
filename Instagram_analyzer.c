#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json.h>

#define MAX_POSTS 10
#define API_KEY "542f88af27msh098aed0cda95fcbp1d56adjsn246cfa3c3fb7"



// Structure to store post data
typedef struct {
    char username[100];
    int likes;
    int comments;
    int engagement;
    char caption[500];
    
} InstagramPost;

// Structure for API response
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Callback function to handle API response
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("Not enough memory!\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i] != '\0'; i++)
        str[i] = tolower((unsigned char)str[i]);
}

// Check if keyword exists in text
int contains(char text[], const char keyword[]) {
    return strstr(text, keyword) != NULL;
}

// Fetch Instagram posts by hashtag
int fetchInstagramPosts(const char *hashtag, InstagramPost *posts) {
    CURL *curl;
    CURLcode res;
    struct MemoryStruct chunk;
    
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
CURL *hnd = curl_easy_init();

curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
curl_easy_setopt(hnd, CURLOPT_URL, "https://instagram-looter2.p.rapidapi.com/search?query=javan&select=users");

struct curl_slist *headers = NULL;
headers = curl_slist_append(headers, "x-rapidapi-key: 542f88af27msh098aed0cda95fcbp1d56adjsn246cfa3c3fb7");
headers = curl_slist_append(headers, "x-rapidapi-host: instagram-looter2.p.rapidapi.com");
curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

CURLcode ret = curl_easy_perform(hnd);

        
        int post_count = 0;
        if(res == CURLE_OK) {
            // Parse JSON response
            struct json_object *parsed_json;
            struct json_object *data;
            struct json_object *items;
            
            parsed_json = json_tokener_parse(chunk.memory);
            
            if(json_object_object_get_ex(parsed_json, "data", &data)) {
                if(json_object_object_get_ex(data, "items", &items)) {
                    int array_len = json_object_array_length(items);
                    
                    for(int i = 0; i < array_len && i < MAX_POSTS; i++) {
                        struct json_object *item = json_object_array_get_idx(items, i);
                        struct json_object *user, *likes_obj, *comments_obj, *caption_obj;
                        
                        // Extract data
                        if(json_object_object_get_ex(item, "user", &user)) {
                            struct json_object *username;
                            json_object_object_get_ex(user, "username", &username);
                            strncpy(posts[i].username, json_object_get_string(username), 99);
                        }
                        
                        if(json_object_object_get_ex(item, "like_count", &likes_obj)) {
                            posts[i].likes = json_object_get_int(likes_obj);
                        }
                        
                        if(json_object_object_get_ex(item, "comment_count", &comments_obj)) {
                            posts[i].comments = json_object_get_int(comments_obj);
                        }
                        
                        // Calculate engagement score
                        posts[i].engagement = posts[i].likes + (posts[i].comments * 2);
                        
                        if(json_object_object_get_ex(item, "caption", &caption_obj)) {
                            struct json_object *text;
                            if(json_object_object_get_ex(caption_obj, "text", &text)) {
                                strncpy(posts[i].caption, json_object_get_string(text), 499);
                            }
                        }
                        
                        post_count++;
                    }
                }
            }
            
            json_object_put(parsed_json);
        }
        
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        free(chunk.memory);
        curl_global_cleanup();
        
        return post_count;
    }
    
    return 0;
}

// Compare function for sorting by engagement (descending)
int compareByEngagement(const void *a, const void *b) {
    InstagramPost *postA = (InstagramPost *)a;
    InstagramPost *postB = (InstagramPost *)b;
    return postB->engagement - postA->engagement;
}

// Display posts sorted by trending
void displayTrendingPosts(InstagramPost *posts, int count) {
    printf("\n=== TOP TRENDING POSTS ===\n\n");
    
    for(int i = 0; i < count && i < 10; i++) {
        printf("Rank #%d\n", i + 1);
        printf("Username: @%s\n", posts[i].username);
        printf("Likes: %d | Comments: %d | Engagement Score: %d\n", 
               posts[i].likes, posts[i].comments, posts[i].engagement);
        printf("Caption: %.100s%s\n", posts[i].caption, 
               strlen(posts[i].caption) > 100 ? "..." : "");
        printf("-------------------------------------------\n\n");
    }
}

int main() {
    char post[500];
    InstagramPost posts[MAX_POSTS];

    printf("Enter post description:\n");
    if (fgets(post, sizeof(post), stdin) != NULL) {
        post[strcspn(post, "\n")] = '\0';
        toLowerCase(post);

        char *hashtag = NULL;
        
        if (contains(post, "#fashion") || contains(post, "clothes") || contains(post, "dress")) {
            printf("Category: FASHION & ACCESSORIES\n");
            hashtag = "fashion";
        }
        else if (contains(post, "#electronics") || contains(post, "mobile") || contains(post, "laptop")) {
            printf("Category: ELECTRONICS\n");
            hashtag = "electronics";
        }
        else if (contains(post, "#ai") || contains(post, "artificial intelligence") || contains(post, "machine learning")) {
            printf("Category: AI & TECHNOLOGY\n");
            hashtag = "ai";
        }
        else if (contains(post, "#sports") || contains(post, "football") || contains(post, "cricket")) {
            printf("Category: SPORTS\n");
            hashtag = "sports";
        }
        else {
            printf("Category: OTHERS\n");
        }
        
        // Fetch and display trending posts if category identified
        if(hashtag != NULL) {
            printf("\nFetching trending posts for #%s...\n", hashtag);
            
            int post_count = fetchInstagramPosts(hashtag, posts);
            
            if(post_count > 0) {
                // Sort by engagement score (trending)
                qsort(posts, post_count, sizeof(InstagramPost), compareByEngagement);
                
                // Display top trending posts
                displayTrendingPosts(posts, post_count);
            } else {
                printf("Could not fetch posts. Check API key and connection.\n");
            }
        }
    }

    return 0;
}

// COMPILATION INSTRUCTIONS:
// Install required libraries:
// Ubuntu/Debian: sudo apt-get install libcurl4-openssl-dev libjson-c-dev
// macOS: brew install curl json-c
//
// Compile with:
// gcc -o instagram_analyzer instagram_analyzer.c -lcurl -ljson-c
//
// Run:
// ./instagram_analyzer