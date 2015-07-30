/**
 * QuickSort example - sorting /etc/passwd entries using fullnames
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <sys/types.h>
#include <pwd.h>

#include <assert.h>


struct my_entry {
       char *login;
       char *full_name;
};

static struct my_entry *create_my_entry(struct passwd *pwd){
	struct my_entry *p = (struct my_entry*)malloc(sizeof(struct my_entry));
        char *p_comma;

        if ( p == NULL ){
		return p;
        }
        memset(p,0,sizeof(struct my_entry));
        p->login = strdup( pwd->pw_name );
        if ( pwd->pw_gecos != NULL && strlen( pwd->pw_gecos ) > 0 ){
		p->full_name = strdup ( pwd->pw_gecos ); 
        } else {
	        p->full_name = strdup(p->login); 
        }
        /* terminate string on  first comma in fullnames */
	if ( (p_comma = strchr( p->full_name,',' ))!=NULL ){
		p_comma = '\0';
        }
	return p;
}

static void free_my_entry(struct my_entry *p){
       assert(p != NULL );
       assert( p->login != NULL);
       assert( p->full_name != NULL);
       free (p->login); p->login = NULL;
       free (p->full_name); p->full_name = NULL;
       free (p);
}

/**
 * comparator to sort by fullname - we expect format
 * as 'FirstName LastName'
 */
static int my_compare (const void *v1, const void *v2){
	// NOTE: we got pointer to pointer !!!
	const struct my_entry **e1 = (const struct my_entry**)v1;
	const struct my_entry **e2 = (const struct my_entry**)v2;
	int r;
	const char *p1 = (*e1)->full_name;
	const char *p2 = (*e2)->full_name;

	// look for LastName
	const char *s1 = strchr(p1,' ');
	if ( s1 == NULL ){
		s1 = p1;
	} else {
		s1 ++;
	}
	// look for LastName
	const char *s2 = strchr(p2,' ');
	if ( s2 == NULL ){
		s2 = p2;
	} else {
		s2 ++;
	}
	r = strcasecmp(s1,s2);
//	printf("strcasecmp(%s,%s)=%d\n",s1,s2,r);
	if ( r!= 0){
		return r;
	}	

        r = strcasecmp(p1,p2);
//	printf("strcasecmp2(%s,%s)=%d\n",p1,p2,r);
	return r;
}


int main(int argc, char **argv){
        struct passwd *pwd;

        struct my_entry **arr;
        int n=0;
        int capacity = 16;

        int i;

        arr = (struct my_entry**)malloc( sizeof(struct my_entry*) * capacity );
        if ( arr == NULL ){
		perror("malloc");
                return 1;
        }

	while ( (pwd = getpwent())!=NULL ){
                assert(pwd->pw_name != NULL );
                struct my_entry *p = create_my_entry(pwd);
		if ( n == capacity ){
                     capacity = capacity * 2;    
                     arr = (struct my_entry**)realloc(arr, sizeof(struct my_entry*) * capacity );
		     if ( arr == NULL ){
                         perror ("realloc");
                         return 1;
                     }
                }
//		printf("n=%d,capacity=%d,p=%s\n",n,capacity,p->login);
                arr[n++] = p;
	}
        endpwent();

	qsort(arr,n,sizeof(struct my_entry*),my_compare);

        for(i=0;i<n;i++){
		printf("%-16s %s\n",arr[i]->login,arr[i]->full_name);
        	free_my_entry( arr[i] ); arr[i] = NULL;
        }
        free(arr); arr = NULL;

	return EXIT_SUCCESS;
}
