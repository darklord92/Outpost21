///scr_cpacket_character_lock( connection id, entity locked to);
var send_buffer = buffer_create(1,buffer_grow,1);
buffer_write(send_buffer,buffer_u8,argument0);
buffer_write(send_buffer,buffer_u16, enum_client_packets.character_locked);
buffer_write(send_buffer,buffer_u32, argument1);
ds_queue_enqueue(global.packet_batch_queue,send_buffer);
