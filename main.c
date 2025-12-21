/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2025/12/03 22:49:59 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>


#include "philo.h"

typedef struct s_rule
{
	int number_of_philo;
	int time_to_die;
	int time_to_eat;
	int time_to_sleep;
	int number_of_times_each_philo_eat;
	long start_time;
} t_rule;

typedef struct s_philo
{
	int id;
	pthread_t thread;
	// long start_time;
	long last_eat_time;
	int meal_count;
	pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
	void *data;
} t_philo;

typedef struct s_data
{
	t_rule rule_data;
	t_philo *philos_data;
} t_data;


long time_in_ms(void)
{
	struct timeval time;
	gettimeofday(&time, NULL);
	long start = time.tv_sec * 1000000 + time.tv_usec;

	return (start / 1000);
}

int ft_atoi(char *str)
{
	int i;
	int res;

	i = 0;
	res = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	if (res <= 0)
	{
		printf("Error: Please enter a positive integer.\n");
		exit(1);
	}
	return (res);
}

void insert_rule(t_rule *rule, int ac, char **argv)
{
	rule->number_of_philo = ft_atoi(argv[1]);
	rule->time_to_die = ft_atoi(argv[2]);
	rule->time_to_eat = ft_atoi(argv[3]);
	rule->time_to_sleep = ft_atoi(argv[4]);
	if (ac == 5)
		rule->number_of_times_each_philo_eat = ft_atoi(argv[5]);
	else
		rule->number_of_times_each_philo_eat = -1;
}

void *routine(void *arg)
{
	t_philo *p = (t_philo *)arg;
	t_data *data = p->data;
	t_rule rule = data->rule_data;
	// printf("this is a routine ");
	// printf("%d %p フォーク左:%p フォーク右:%p", p->id, p->thread, p->right_fork, p->left_fork);
	// printf(" %ld \n", time_in_ms() - p->start_time);

	// pthread_mutex_lock(p->right_fork);
	// printf("%ld %d has taken a fork %d\n", time_in_ms() - p->start_time, p->id, p->id);
	// pthread_mutex_lock(p->left_fork);
	// printf("%ld %d has taken a fork %d\n", time_in_ms() - p->start_time, p->id, p->id);

	while(1) {
		if (p->id % 2 == 0)
		{
			pthread_mutex_lock(p->right_fork);
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n",time_in_ms() - rule.start_time, p->id);
			printf("%ld %d is eating\n",time_in_ms() - rule.start_time, p->id);
			usleep(rule.time_to_eat);
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
			printf("%ld %d is sleeping\n",time_in_ms() - rule.start_time, p->id);
			usleep(rule.time_to_sleep);
			// usleep(食べる時間) ->　printf(寝る)　-> usleep(寝る時間)
		}
		else 
		{
			pthread_mutex_lock(p->right_fork);
			pthread_mutex_lock(p->left_fork);
			printf("%ld %d has taken a fork\n",time_in_ms() - rule.start_time, p->id);
			printf("%ld %d is eating\n",time_in_ms() - rule.start_time, p->id);
			usleep(rule.time_to_eat);
			pthread_mutex_unlock(p->right_fork);
			pthread_mutex_unlock(p->left_fork);
			printf("%ld %d is sleeping\n",time_in_ms() - rule.start_time, p->id);
			usleep(rule.time_to_sleep);
		}
		pthread_mutex_unlock(p->right_fork);
		pthread_mutex_unlock(p->left_fork);
	}
		
	return NULL;
}

void make_thread(t_data *data)
{
	// t_philo philos[rule->number_of_philo];
	pthread_mutex_t forks[data->rule_data.number_of_philo];

	int i = 0;
	data->rule_data.start_time = time_in_ms();
	while (i < data->rule_data.number_of_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		data->philos_data[i].id = i;
		data->philos_data[i].right_fork = &forks[i];
		data->philos_data[i].left_fork = &forks[(i + 1) % data->rule_data.number_of_philo];
		// data->philos_data[i].start_time = data->rule_data.start_time;
		data->philos_data[i].data = data;
		pthread_create(&data->philos_data[i].thread, NULL, routine, &data->philos_data[i]);
		i++;
	}
	for (i = 0; i < data->rule_data.number_of_philo; i++)
		pthread_join(data->philos_data[i].thread, NULL);
}

int main(int argc, char **argv)
{
	t_data data;
	// t_rule rule;

	if (argc != 5 && argc != 6)
		return (0);

	printf("start\n");

	insert_rule(&data.rule_data, argc - 1, argv);
	data.philos_data = malloc(sizeof(t_philo) * data.rule_data.number_of_philo);
	
	make_thread(&data);



	// printf("%d %d %d %d", rule.number_of_philo, rule.time_to_die,
	// 	   rule.time_to_eat, rule.time_to_sleep);
	// if (argc == 6)
	// 	printf(" %d", rule.number_of_times_each_philo_eat);

	return (0);
}
