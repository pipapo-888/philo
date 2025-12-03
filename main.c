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

#include "philo.h"

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
	printf("this is a routine ");
	printf("%d %ld\n", p->id, p->thread);
}

void make_thread(t_rule *rule)
{
	t_philo philos[rule->number_of_philo];
	int i = 0;
	while (i < rule->number_of_philo)
	{
		philos[i].id = i;
		pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
		i++;
	}

	for (i = 0; i < rule->number_of_philo; i++)
		pthread_join(philos[i].thread, NULL);
}

int main(int argc, char **argv)
{
	t_rule rule;

	if (argc != 5 && argc != 6)
		return (0);

	insert_rule(&rule, argc - 1, argv);
	make_thread(&rule);

	// printf("%d %d %d %d", rule.number_of_philo, rule.time_to_die,
	// 	   rule.time_to_eat, rule.time_to_sleep);
	// if (argc == 6)
	// 	printf(" %d", rule.number_of_times_each_philo_eat);

	return (0);
}
