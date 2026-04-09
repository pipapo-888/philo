/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_death_thread.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: knomura <knomura@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/08 16:45:08 by knomura           #+#    #+#             */
/*   Updated: 2026/04/09 17:19:59 by knomura          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	is_dead_philo(t_data *data, int i)
{
	return (t_ms()
		- data->philos_data[i].last_eat_time > data->rule_data.time_to_die);
}

static void	print_death(t_data *data, int i)
{
	printf("%ld %d died\n", t_ms() - data->rule_data.start_time,
		data->philos_data[i].id);
}

static int	is_all_ate(t_data *data)
{
	int	count;

	pthread_mutex_lock(&data->all_ate_mtx);
	count = data->all_ate_count;
	pthread_mutex_unlock(&data->all_ate_mtx);
	return (count == data->rule_data.number_of_philo);
}

void	*check_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (!is_all_ate(data))
	{
		i = 0;
		while (i < data->rule_data.number_of_philo)
		{
			pthread_mutex_lock(&data->philos_data[i].eat_time);
			if (is_dead_philo(data, i))
			{
				pthread_mutex_unlock(&data->philos_data[i].eat_time);
				pthread_mutex_lock(&data->death_flag_mtx);
				data->death_flag = 1;
				pthread_mutex_unlock(&data->death_flag_mtx);
				print_death(data, i);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos_data[i].eat_time);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
