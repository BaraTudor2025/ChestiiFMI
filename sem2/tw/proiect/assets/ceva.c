void ReadCountries()
{
    int country_nr;
    int current=0;
    FILE *f;
    char string[100];
    f = fopen ("date.in" , "r");
    if (f == NULL) {
        printf("Error opening file");
        goto close_file;
    }
    if (fgets (string , 100 , f) == NULL )
        printf("couldn't read from file");
        goto close_file;
    }
    country_nr = atoi(string);
close_file:
    fclose (f);
}

#define READ_FROM_FILE(VAR, BYTES) \
    
    if (fgets (VAR, BYTES, f) == NULL ) \
        printf("couldn't read from file");\
        goto close_file; \
    }

int compScore(const void* a, const void* b){
    Values* va = a;
    Values* vb = b;
    return va->score - vb->score;
}