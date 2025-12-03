#ifndef PHILO_H
#define PHILO_H

typedef struct s_rule
{
	int number_of_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philo_eat;
} t_rule;

typedef struct s_philo
{
	int id;
	pthread_t thread;
} t_philo;

#endif