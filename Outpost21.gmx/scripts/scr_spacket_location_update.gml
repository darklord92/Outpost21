///scr_spacket_location_update( entity id, new x, new y);
var send_buffer = buffer_create(1,buffer_grow,1);
buffer_write(send_buffer,buffer_u16, enum_server_packets.movement_location_update);
buffer_write(send_buffer,buffer_u32, argument0);
buffer_write(send_buffer,buffer_f32, argument1);
buffer_write(send_buffer,buffer_f32, argument2);
network_send_packet(global.client_socket, send_buffer, buffer_get_size(send_buffer));
buffer_delete(send_buffer);
