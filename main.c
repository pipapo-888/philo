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
	long start_time;
	long last_eat_time;
	int meal_count;
	pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;  
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

	return (start);
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
	// printf("this is a routine ");
	// printf("%d %p フォーク左:%p フォーク右:%p", p->id, p->thread, p->right_fork, p->left_fork);
	// printf(" %ld \n", time_in_ms() - p->start_time);

	// pthread_mutex_lock(p->right_fork);
	// printf("%ld %d has taken a fork %d\n", time_in_ms() - p->start_time, p->id, p->id);
	// pthread_mutex_lock(p->left_fork);
	// printf("%ld %d has taken a fork %d\n", time_in_ms() - p->start_time, p->id, p->id);

	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(p->right_fork);
		pthread_mutex_lock(p->left_fork);
		printf("%ld %d has taken a fork\n",time_in_ms() - p->start_time, p->id);
		// usleep(食べる時間) ->　printf(寝る)　-> usleep(寝る時間)
	}
	else 
	{
		pthread_mutex_lock(p->left_fork);
		pthread_mutex_lock(p->right_fork);
		printf("%ld %d has taken a fork\n",time_in_ms() - p->start_time, p->id);
	}
	pthread_mutex_unlock(p->right_fork);
	pthread_mutex_unlock(p->left_fork);

	return NULL;
}

void make_thread(t_rule *rule)
{
	t_philo philos[rule->number_of_philo];
	pthread_mutex_t forks[rule->number_of_philo];

	rule->start_time = time_in_ms();
	int i = 0;
	while (i < rule->number_of_philo)
	{
		pthread_mutex_init(&forks[i], NULL);
		philos[i].id = i;
		philos[i].right_fork = &forks[i];
		philos[i].left_fork = &forks[(i + 1) % rule->number_of_philo];
		philos[i].start_time = rule->start_time;
		pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
		i++;
	}
	for (i = 0; i < rule->number_of_philo; i++)
		pthread_join(philos[i].thread, NULL);
}

int main(int argc, char **argv)
{
	t_data data;
	// t_rule rule;

	if (argc != 5 && argc != 6)
		return (0);

	insert_rule(&data.rule_data, argc - 1, argv);
	
	make_thread(&data.rule_data);




	// printf("%d %d %d %d", rule.number_of_philo, rule.time_to_die,
	// 	   rule.time_to_eat, rule.time_to_sleep);
	// if (argc == 6)
	// 	printf(" %d", rule.number_of_times_each_philo_eat);

	return (0);
}
