
## TODO (実装項目)


- [ ] **死亡フラグ (`is_dead`)** の追加 — 哲学者が死んだとき全スレッドを止める共有フラグ
- [ ] **print用ミューテックス** — `printf` が複数スレッドから同時に呼ばれないよう保護する
- [ ] **全員食事完了の検知** — `number_of_times_each_philo_eat` 達成時にシミュレーション終了
- [ ] **`eat_time` ミューテックスの初期化** — `pthread_mutex_init(&philos_data[i].eat_time, NULL)` が未実装
- [ ] **スレッドの join** — `check_death` 終了後に全哲学者スレッドを `pthread_join` する（現在 `return` で到達不能）
- [ ] **ミューテックスのクリーンアップ** — `pthread_mutex_destroy` の呼び出し
- [ ] **メモリ解放** — `free(data.philos_data)` の呼び出し
- [ ] **哲学者1人の特殊ケース** — フォークが1本しかないので食べられず死ぬ処理
- [ ] **デバッグ用 `printf("start\n")` の削除** — `main.c:153`
- [ ] **ルーティンの共通化** — `main.c` の `routine` 内でほぼ同じコードが奇数/偶数で重複している
- [ ]飯食う時間が　死亡までの時間より長いケース
- [ ] is_running フラグを追加して routine を終了できるようにする（check_death が死亡検知したら is_running=0 → while(is_running) で全スレッド終了 → pthread_join で回収）