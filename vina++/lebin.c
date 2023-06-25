#include <stdio.h>

int main(){
	FILE* archive = fopen("archive.bin", "rb");
	char buffer;
	while (fread(&buffer, sizeof(int), 1, archive) > 0)
		printf("%d", buffer);

	return 0;
}