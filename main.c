/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2025/11/18 22:23:17 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "philo.h"

typedef struct s_philo
{
	int id;
	pthread_t thread;
} t_philo;

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

void insert_value(t_rule *rule, int ac, char **argv)
{
	rule->number_of_philo = ft_atoi(argv[1]);
	rule->time_to_die = ft_atoi(argv[2]);
	rule->time_to_eat = ft_atoi(argv[3]);
	rule->time_to_sleep = ft_atoi(argv[4]);
	if (ac == 5)
		rule->number_of_times_each_philo_eat = ft_atoi(argv[5]);
}

void make_thread(t_rule *rule)
{
	t_philo philos[rule->number_of_philo];
	int i = 0;
	while (i < rule->number_of_philo)
	{
		pthread_create(philos[i].thread, )

		i++;
	}
}

int main(int argc, char **argv)
{
	t_rule rule;

	if (argc != 5 && argc != 6)
		return (0);

	insert_value(&rule, argc - 1, argv);
	make_thread(&rule);

	printf("%d %d %d %d", rule.number_of_philo, rule.time_to_die,
		   rule.time_to_eat, rule.time_to_sleep);
	if (argc == 6)
		printf(" %d", rule.number_of_times_each_philo_eat);
	return (0);
}
