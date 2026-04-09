/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/04/09 14:58:27 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data(t_data *data)
{
	int	ret;

	data->death_flag = 0;
	data->all_ate_count = 0;
	ret = pthread_mutex_init(&data->death_flag_mtx, NULL);
	if (ret)
		return (write(2, "Error: mutex init failed\n", 25), ret);
	ret = pthread_mutex_init(&data->all_ate_mtx, NULL);
	if (ret)
	{
		pthread_mutex_destroy(&data->death_flag_mtx);
		write(2, "Error: mutex init failed\n", 25);
		return (ret);
	}
	data->philos_data = malloc(sizeof(t_philo)
			* data->rule_data.number_of_philo);
	if (!data->philos_data)
	{
		pthread_mutex_destroy(&data->death_flag_mtx);
		pthread_mutex_destroy(&data->all_ate_mtx);
		write(2, "Error: malloc failed\n", 21);
		return (1);
	}
	return (0);
}

void	insert_rule(t_rule *rule, int ac, char **argv)
{
	rule->number_of_philo = ft_atoi(argv[1]);
	rule->time_to_die = ft_atoi(argv[2]);
	rule->time_to_eat = ft_atoi(argv[3]);
	rule->time_to_sleep = ft_atoi(argv[4]);
	if (ac == 6)
		rule->number_of_times_each_philo_eat = ft_atoi(argv[5]);
	else
		rule->number_of_times_each_philo_eat = -1;
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		ret;

	if (argc != 5 && argc != 6)
		return (0);
	insert_rule(&data.rule_data, argc, argv);
	ret = init_data(&data);
	if (ret)
		return (ret);
	make_thread(&data);
	return (0);
}
