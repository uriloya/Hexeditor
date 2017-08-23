#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <malloc.h>

void debug(); 
void setFileName();
void setUnitSize();
void quit();
void fileDisplay();
void loadIntoMemory();
void saveIntoFile();
void fileModify();

int deb = 0;
int size = 1;
int first = 1;
char* filename = "abc";
char* data_pointer = NULL;
char* password = "pass\n";

struct fun_desc
{
    char *name;
    void (*fun)();
};

void debug()
{
    char str[100];
    printf("Enter password\n");
    fgets(str, 100, stdin);
    if (strcmp(str, password) != 0) {
        printf("wrong password\n");
        return;
    }
    if(deb == 0)
    {
        printf("Debug flag now on\n");
        deb = 1;
    }
    else
    {
        printf("Debug flag now off\n");
        deb = 0;
    }
}

void setFileName()
{
    char buffer[100];
    if(!fgets(buffer, 100, stdin))
        printf("bad input\n");
    else
    {
        if(first != 1)
            free(filename);
        else
            first = 0;
        filename = malloc(strlen(buffer)+1);
        strcpy(filename,buffer);
        strtok(filename, "\n");
        if(deb == 1)
            printf("Debug: file name set to %s\n", filename);
    }
}

void setUnitSize()
{
    char buffer[3];
    if(!fgets(buffer, 3, stdin))
        printf("bad input\n");
    else
    {
        if(buffer[0] == '1')
            size = 1;
        else if(buffer[0] == '2')
            size = 2;
        else if(buffer[0] == '4')
            size = 4;
        if(deb == 1)
            printf("Debug: set size to %d\n", size);
    } 
}


void fileDisplay()
{
    if (filename == NULL){
        printf("Error, no file was found\n");
        return;
    }

    FILE *file = fopen(filename, "r"); 
    if (file == NULL)
    {
        perror(filename);
        return;
    } 
    char buf[100];
    int location;
    int length;
    printf("Please enter <location> <length>\n");
    if(!fgets(buf, 100, stdin))
        printf("bad input\n");
    
    sscanf(buf, "%x %d", &location, &length);
    
    if(deb == 1)
        printf("Debug: location set to %x\nlength set to %d\n", location, length);
    char *disp = malloc(size*length);
    fseek(file, location, SEEK_SET);

    printf("size is: %d\n", size);
    printf("length is: %d\n", length);
    int r = fread(disp, 1, size*length, file);
    printf("number of bytes read: %d\n", r);
    fclose(file);
    char *tmp = disp;
    hexPrint(tmp, length);
    decPrint(tmp, length);
    free(disp);
}   
void decPrint(char *disp, int length)
{
    printf("Decimal Representation:\n");
    int l = length;
    int b;
    while (l > 0)
    {
        int chunk = 0;
        int base = 100;
        int rounds = size;
        disp = disp + size - 1;
        while (rounds > 0)
        {
            b = *disp;
            if (b < 0) b = 256 + b;     /* when b points to e0, the decimal value is 32 instead of 224 (????) */
            if (rounds != 1)
                b = b*256;
            chunk += b;
            disp--;
            rounds--;
        }
        printf("%d ", chunk);
        disp = disp + size + 1;
        l--;
    }
    printf("\n");
}
void hexPrint(char *disp, int length)
{   
    printf("Hexadecimal Representation:\n");
    int l = length;
    while (l > 0)
    {
        int rounds = size;
        disp = disp + size - 1;
        while (rounds > 0)
        {
            printf("%02x", *disp & 0xff);
            disp--;
            rounds--;
        }
        printf(" ");
        disp = disp + size + 1;
        l--;
    }
    printf("\n");
}


void quit()
{
    free(data_pointer);
    if(first != 1)
        free(filename);
    exit(0);
}

void loadIntoMemory()
{
    if (filename == NULL){
        printf("Error, no file was found\n");
        return;
    }

    FILE *file = fopen(filename, "r"); 
    if (file < 0)
    {
        perror(filename);
        return;
    } 
    char buf[100];
    int location;
    int length, mem_adress;
    printf("Please enter <mem-address> <location> <length>\n");
    if(!fgets(buf, 100, stdin))
        printf("bad input\n");
    
    sscanf(buf, "%x %x %d", &mem_adress, &location, &length);

    if (mem_adress == 0) mem_adress = data_pointer;
    
    if (data_pointer != NULL) free(data_pointer);
    
    data_pointer = malloc(length*size);

    if(deb == 1) printf("Debug: filename set to %s\ndata pointer set to %x\nlocation set to %x\nlength set to %d\n", filename, data_pointer, location, length);
    fseek(file, location, SEEK_SET);
    int r = fread(data_pointer, 1, size*length, file);
    char *tmp = data_pointer;
    if (r > 0) printf("Loaded %d bytes into %x\n", r, mem_adress);
    fclose(file);

}


void saveIntoFile()
{
    int src_add;
    int location;
    int length;
    char buf[100];
    int file_len;
    
    if (filename == NULL){
        printf("Error, no file was found\n");
        return;
    }

    FILE *file = fopen(filename, "r+"); 

    if (file < 0)
    {
        perror(filename);
        return;
    } 

    /* GET THE LENGTH OF THE FILE */
    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    rewind(file); 

    /* PROMPT */
    printf("Please enter <source-address> <target-location> <length>\n");
    if(!fgets(buf, 100, stdin))
        printf("bad input\n");
    
    sscanf(buf, "%x %x %d", &src_add, &location, &length);
    if (location > file_len){
        printf("location is: %x and file length is %x \n", location, file_len);
        printf("location is out of bounds\n");
        return;
    }
    
    if(deb == 1) printf("Debug: filename set to %s\nsource address set to %x\ntarget location set to %x\nlength set to %d\n", filename, src_add, location, length);
    

    fseek(file, location, SEEK_SET);

    fwrite(data_pointer, length*size, 1, file);
    fclose(file);

}

void fileModify()
{
    int location;
    char *value;
    char buf[100];
    int file_len;

    if (filename == NULL){
        printf("Error, no file was found\n");
        return;
    }

    FILE *file = fopen(filename, "r+"); 

    if (file < 0)
    {
        perror(filename);
        return;
    } 

    fseek(file, 0, SEEK_END);
    file_len = ftell(file);
    rewind(file); 

    printf("Please enter <location> <val>\n");
    if(!fgets(buf, 100, stdin))
        printf("bad input\n");
    
    sscanf(buf, "%x %x", &location, &value);
    if(deb == 1) printf("Debug: location set to %x\nvalue set to %x\n", location, value);

    if (location+size > file_len){
        printf("location is: %x and file length is %x \n", location, file_len);
        printf("location is out of bounds\n");
        return;
    }
    
    fseek(file, location, SEEK_SET);
    fwrite(&value, size, 1, file);

    fclose(file);

}


int digCount(int x)
{
    int ans = 0;
    while (x != 0){
        x /= 10;
        ans++;
    }
    return ans;
}


int main(int argc, char **argv)
{
    struct fun_desc menu[] = 
    { 
        { "Debug", debug }, 
        { "Set File Name", setFileName },
        { "Set Unit Size", setUnitSize },
        { "File Display", fileDisplay },
        { "Load Into Memory", loadIntoMemory },
        { "Save Into File", saveIntoFile },
        { "File Modify", fileModify },
        { "Quit", quit } 
    }; 

    char input[3];
    int i = 0;
    int op;
    int numOfOps = 7;

    while(1) 
    { 
        printf("Choose action:\n"); 
        for(i = 0; i <= numOfOps; i++)
        {
            printf("%i-%s\n",i ,menu[i].name);
        }

        if(!fgets(input, 3, stdin))
            break;
        op = atoi(input);
        if(op >= 0 && op <= numOfOps)
        {
            menu[op].fun();
        }
        else
        {
            printf("Not within bounds\n");
        }
    }
    return 0; 
}
