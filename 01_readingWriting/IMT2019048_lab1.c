# include <stdio.h> 

struct student {
  int rollnum;
  char name[30];
  int age;
};

int save_num_text( char *filename ) {
	
	// Opening file in write mode 
	FILE * fp = fopen(filename, "w");
	if(fp == NULL) {
		return 1;
	}

	for(int num = 1; num <= 20; num++) {
		fprintf(fp, "%d\n", num);
	}
	fclose(fp);
	return 0;
}

int read_num_text( char *filename ) {
 	
 	// Opening file in read mode 
 	FILE * fp = fopen(filename, "r");
	if(fp == NULL) {
		return 1;
	}

	for(int i = 1; i <= 20; i++) {
		int num;
		fscanf(fp, "%d", &num);
		printf("%d\n", num);
	}
	fclose(fp);
 	return 0;
}

int save_struct_text( char *filename ) {
  	
  	// Opening file in write mode 
  	FILE * fp = fopen(filename, "w");
	if(fp == NULL) {
		return 1;
	}

  	for(int i = 0; i < 5; i++) {

  		struct student st;
  		st.rollnum = i+1;
  		sprintf(st.name, "student%d", i+1);
  		st.age = 20+(i%2);

  		fprintf(fp, "%d %s %d\n", st.rollnum, st.name, st.age);
  	}
  	return 0;
}

int read_struct_text( char *filename ) {
  	
  	// Opening file in read mode 
  	FILE * fp = fopen(filename, "r");
	if(fp == NULL) {
		return 1;
	}

  	for(int i = 0; i < 5; i++) {
  		struct student st;
  		fscanf(fp, "%d %s %d", &st.rollnum, st.name, &st.age);
  		printf("%d %s %d\n", st.rollnum, st.name, st.age);
  	}
  	return 0;
}







int save_num_binary( char *filename ) {
	
	// Opening file in write binary mode 
	FILE * fp = fopen(filename, "wb");
	if(fp == NULL) {
		return 1;
	}

	for(int num = 1; num <= 20; num++) {
		fwrite(&num, sizeof(num), 1, fp);
	}
	fclose(fp);
	return 0;
}

int read_num_binary( char *filename ) {
	
	// Opening file in read binary mode 
	FILE * fp = fopen(filename, "rb");
	if(fp == NULL) {
		return 1;
	}

	for(int i = 1; i <= 20; i++) {
		int num;
		fread(&num, sizeof(num), 1, fp);
		printf("%d\n", num);
	}
	fclose(fp);
	return 0;
}

int save_struct_binary( char *filename ) {
  	
  	// Opening file in write binary mode 
  	FILE * fp = fopen(filename, "wb");
	if(fp == NULL) {
		return 1;
	}

  	for(int i = 0; i < 5; i++) {

  		struct student st;
  		st.rollnum = i+1;
  		sprintf(st.name, "student%d", i+1);
  		st.age = 20+(i%2);

  		fwrite(&st, sizeof(st), 1, fp);
  	}
  	return 0;
}

int read_struct_binary( char *filename ) {
  	
  	// Opening file in read binary mode 
  	FILE * fp = fopen(filename, "rb");
	if(fp == NULL) {
		return 1;
	}

  	for(int i = 0; i < 5; i++) {
  		struct student st;
  		fread(&st, sizeof(st), 1, fp);
  		printf("%d %s %d\n", st.rollnum, st.name, st.age);
  	}
  	return 0;
}

