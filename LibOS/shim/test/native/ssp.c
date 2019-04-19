
#include "common.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include <shim_unistd.h>

learned_factors_t* learned;
training_set_t* tset;
recommended_items_t* r_items = NULL;
learning_model_t model = {0};
model_parameters_t params = {0};

int training();
int child_process(int fd);
int pipefds[4];

int main(int argc, char ** argv)
{
	printf("start ssp\n");

	struct sockaddr_in servaddr, cli;
	int serv_socket, i, connfd;
	unsigned len=0;

	char bytec = 0, bytes = 0;
	int check;

	printf("training start\n");
	training();

	printf("socket start\n");
	if ((serv_socket =socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))==-1) {
		fprintf(stderr, "socket() failed\n");
		exit(1);
	}

	memset((char *) &servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SRV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding newly created socket to given IP and verification 
	if ((bind(serv_socket , (struct sockaddr *)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(1); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(serv_socket, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(1); 
	} 

	len = sizeof(cli); 

	pid_t pid;

	fflush(stdout);

	pipe(&pipefds[0]);

	while(1){

		fflush(stdout);

		pid = fork();

		if (pid > 0){ // parent
			printf("parent (pid: %d) *** \n", getpid());

			read(pipefds[0], &bytec, 1);
			// bytes = connfd;
			// close (connfd);

			/*		
						printf("Server waiting..\n"); 
						waitpid(pid, NULL, 0);
			 */
			printf("Done child..\n"); 
		}
		else if (pid == 0) { // child

			connfd = accept(serv_socket, (struct sockaddr *)&cli, &len);
			if (connfd < 0){
				perror("server accept failed..."); 
				exit(1); 
			}
			write(pipefds[1], &bytes, 1);
			close(pipefds[0]);
			close(pipefds[1]);

			printf("server accept the client...\n"); fflush(stdout);
			printf("child\n", getpid()); fflush(stdout);
			close(serv_socket);

			check = bytec;
			child_process(connfd);

			exit(0);
		}
		else {
			printf("failure..\n");
		}

	}

	printf("end of program\n");
	close(serv_socket);

	return 0;

}

int child_process(int fd){

	rating_estimator_parameters_t rating_params = { 0, 0, learned, tset};

	char buf[BUFLEN]= {0, };

	int ret = read(fd, buf, BUFLEN);
	//  read_nonuser_data(fd, buf, 6000-4096);

	printf("read %d data\n", ret);

	// Now let's try this decision_function on some samples we haven't seen before.

	double one, two;

	char *p = buf;
	memcpy(&one, p, sizeof(double));
	p += sizeof(double);
	memcpy(&two, p, sizeof(double));
	p += sizeof(double);

	printf(	"users [0] item [0], rating = %f \n",
			estimate_rating_from_factors(&rating_params, &model));

	rating_params.user_index = 0;
	rating_params.item_index = 1;

	printf(	"users [0] item [1], rating = %f \n",
			estimate_rating_from_factors(&rating_params, &model));

	rating_params.user_index = 0;
	rating_params.item_index = 2;

	printf(	"users [0] item [2], rating = %f \n", 
			estimate_rating_from_factors(&rating_params, &model));

	rating_params.user_index = 1;
	rating_params.item_index = 1;

	printf(	"users [1] item [1], rating = %f \n", 
			estimate_rating_from_factors(&rating_params, &model));

	rating_params.user_index = 1;
	rating_params.item_index = 0;

	printf(	"users [1] item [0], rating = %f \n",
			estimate_rating_from_factors(&rating_params, &model));

	// rating_params.user_index = 0;
	r_items = recommend_items(&rating_params, &model);

	for (int i = 0; i < r_items->items_number; i++)
	{
		printf("%f \n", r_items->items[i].rating);
	}


	// TODO
	write(fd, buf, BUFLEN);

	close(fd);

	free_recommended_items(r_items);
}

int training(){

	params.items_number = 3;
	params.users_number = 2;
	params.training_set_size = 5;
	params.dimensionality = 128;
	params.iteration_number = 5000;
	params.lambda = 0.005f;
	params.step = 0.0005f;
	params.lambda_bias = 0.02f;
	params.step_bias = 0.001f;

	//Use the basic matrix factorization model
	model.learning_algorithm = learn_mf_bias;
	model.rating_estimator   = estimate_rating_mf_bias;
	model.parameters	 = params;

	//Learning
	//Initialize a training set
	tset = init_training_set(&params);

	set_known_rating(0, 0, 4, tset);
	set_known_rating(0, 1, 1, tset);
	set_known_rating(0, 2, 6, tset);

	set_known_rating(1, 1, 1, tset);
	set_known_rating(1, 0, 2, tset);

	compile_training_set(tset);

	learned = learn(tset, &model);
}
