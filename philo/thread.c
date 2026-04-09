#include "philo.h"

static void	destroy_forks_and_exit(t_data *data, int count, int status)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	free(data->forks);
	free(data->philos_data);
	exit(status);
}

static void	clean_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->rule_data.number_of_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		pthread_mutex_destroy(&data->philos_data[i].eat_time);
		i++;
	}
	pthread_mutex_destroy(&data->death_flag_mtx);
	pthread_mutex_destroy(&data->all_ate_mtx);
	free(data->forks);
	free(data->philos_data);
}

void	make_thread(t_data *data)
{
	int	i;
	int	ret;

	data->forks = malloc(sizeof(pthread_mutex_t)
			* data->rule_data.number_of_philo);
	if (!data->forks)
		exit(1);
	i = 0;
	data->rule_data.start_time = t_ms();
	while (i < data->rule_data.number_of_philo)
	{
		ret = pthread_mutex_init(&data->forks[i], NULL);
		if (ret)
			destroy_forks_and_exit(data, i, ret);
		data->philos_data[i].id = i + 1;
		data->philos_data[i].right_fork = &data->forks[i];
		data->philos_data[i].left_fork = &data->forks[(i + 1)
			% data->rule_data.number_of_philo];
		data->philos_data[i].data = data;
		data->philos_data[i].last_eat_time = data->rule_data.start_time;
		ret = pthread_mutex_init(&data->philos_data[i].eat_time, NULL);
		if (ret)
			destroy_forks_and_exit(data, i + 1, ret);
		ret = pthread_create(&data->philos_data[i].thread, NULL, routine,
				&data->philos_data[i]);
		if (ret)
			destroy_forks_and_exit(data, i + 1, ret);
		i++;
	}
	pthread_create(&data->thread, NULL, check_death, data);
	pthread_join(data->thread, NULL);
	
	i = 0;
	while (i < data->rule_data.number_of_philo)
	{
		pthread_join(data->philos_data[i].thread, NULL);
		i++;
	}
	clean_all(data);
}
