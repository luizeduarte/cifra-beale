#include <stdio.h>

int main(){
	FILE* archive = fopen("archive.bin", "rb");
	char buffer;
	while (fread(&buffer, sizeof(char), 1, archive) > 0)
		printf("%c", buffer);

	return 0;
}