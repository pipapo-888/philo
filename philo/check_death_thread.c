#include "philo.h"

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
			if (t_ms()
				- data->philos_data[i].last_eat_time > data->rule_data.time_to_die)
			{
				pthread_mutex_unlock(&data->philos_data[i].eat_time);
				pthread_mutex_lock(&data->death_flag_mtx);
				data->death_flag = 1;
				pthread_mutex_unlock(&data->death_flag_mtx);
				printf("%ld %d died\n", t_ms() - data->rule_data.start_time,
					data->philos_data[i].id);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philos_data[i].eat_time);
			i++;
		}
		usleep(100);
	}
	return (NULL);
}
