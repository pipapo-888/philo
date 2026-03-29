#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_rule
{
	int				number_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_each_philo_eat;
	long			start_time;
}					t_rule;

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				meal_count;
	long			last_eat_time;
	pthread_mutex_t	eat_time;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	void			*data;
}					t_philo;

typedef struct s_data
{
	t_rule			rule_data;
	t_philo			*philos_data;
	pthread_t		thread;
}					t_data;

#endif