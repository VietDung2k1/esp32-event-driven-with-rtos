# EOS - EVENT DRIVEN WITH RTOS
FreeRTOS porting and event-driven development with FreeRTOS.

![Untitledasdf a (1)](https://github.com/user-attachments/assets/dbe70fed-cf35-42cf-80cf-126fe4ce7fff)

### 1 - Benefits of Event-Driven Development
Utilizing FreeRTOS as the foundation for real-time applications offers the advantage of creating dedicated tasks for various project functions. These tasks can efficiently manage interactions with peripherals, sensors, and other devices.

Adopting an event-driven model in real-time applications offers several key advantages.
* Optimized Resource Management
* Enhanced Flexibility
* Simplified Scalability
* Improved Responsiveness
* Better Modularity
### 2 - The Active Object Pattern
It basically creates a task that continuously waits for events in a private queue. The task also has a dispatcher, which is a function that can handle and execute different actions depending on the type of event.

```c
extern void task_device(void *pvParameters) {

    ...
	
	while (eos_task_receive_msg(TASK_DEVICE_ID, &msg_id, &msg_data, &msg_len)) {
		switch (msg_id) {
		case AC_LIFE_SYSTEM_CHECK: {
			EOS_LOG_INFO(TAG, "AC_LIFE_SYSTEM_CHECK");
			toggle_led_life();
		} break;

        ... Other signal ...

		default:
			break;
		}
		eos_free_msg(msg_data);
	}
}
```
eos_task_receive_msg: waits until a message is sent to the task. The message can come from another task, from a peripheral's interrupt function, from a time service.


Only tasks that receive events need to create a queue for them.
```c
/* Shell: Debug and Config */
eos_create_task(TASK_SHELL_ID);

/* Device: IO Pins */
eos_create_queue(TASK_DEVICE_ID);
eos_create_task(TASK_DEVICE_ID);
```

### Build and Flash the project.

```
idf.py -p PORT build flash monitor
```

### Example output

```

    _/_/_/_/  _/_/_/_/  _/_/_/      _/_/      _/_/_/
   _/        _/        _/    _/  _/    _/  _/
  _/_/_/_/  _/_/_/_/  _/_/_/    _/    _/    _/_/
 _/              _/  _/        _/    _/        _/
_/_/_/_/  _/_/_/_/  _/          _/_/    _/_/_/
Author: VietDung
Build: Nov  2 2024
[00:00:00] [EOS] table task size: 2
[00:00:00] [EOS] wait ...
[00:00:00] [EOS] create task name: _shell, id: 0, stack size: 4096, priority: 1, code: 0x42008628
[00:00:00] [EOS] create queue task name: _device, queue: 0x3fc95050
[00:00:00] [EOS] wait ...
[00:00:00] [DEVICE] STARTED
[00:00:00] [EOS] timer set id: 1, sig: 10, msg: 0x0, len: 0
[00:00:00] [EOS] create task name: _device, id: 1, stack size: 4096, priority: 1, code: 0x42008948

I (567) main_task: Returned from app_main()
[00:00:00] [SHELL] STARTED
[00:00:01] [EOS] timer callback id: 1, sig: 10, msg: 0x0, len: 0
[00:00:01] [EOS] task post id: 1, sig: 10, msg: 0x0, len: 0
[00:00:01] [EOS] task receive copy msg 10 -> 0x0, len: 0
[00:00:01] [DEVICE] AC_LIFE_SYSTEM_CHECK
[00:00:02] [EOS] timer callback id: 1, sig: 10, msg: 0x0, len: 0
[00:00:02] [EOS] task post id: 1, sig: 10, msg: 0x0, len: 0
[00:00:02] [EOS] task receive copy msg 10 -> 0x0, len: 0
[00:00:02] [DEVICE] AC_LIFE_SYSTEM_CHECK

```
### Reference
| Topic | Link |
| ------ | ------ |
| Blog & Tutorial | https://epcb.vn/blogs/ak-embedded-software |
| Where to buy KIT? | https://epcb.vn/products/smart-industrial-iot-gateway-epcb-iec-esp32-s3 |
