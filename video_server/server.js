const express = require('express');
const http = require('http');
const socketIo = require('socket.io');
const path = require('path');

const app = express();
const server = http.createServer(app);
const io = socketIo(server, {
  cors: {
    origin: "*",  // Разрешаем ваш домен
    methods: ["GET", "POST"],
    credentials: true,
    transports: ['websocket', 'polling']
  }
});

const rooms = new Map();

io.on('connection', (socket) => {
    console.log('Новый пользователь:', socket.id);

    socket.on('join-room', (roomId) => {
        socket.join(roomId);
        
        if (!rooms.has(roomId)) {
            rooms.set(roomId, new Set());
        }
        rooms.get(roomId).add(socket.id);

        socket.to(roomId).emit('user-connected', socket.id);

        socket.on('disconnect', () => {
            rooms.get(roomId)?.delete(socket.id);
            socket.to(roomId).emit('user-disconnected', socket.id);
        });

        socket.on('offer', (data) => {
            socket.to(data.target).emit('offer', {
                sdp: data.sdp,
                sender: socket.id
            });
        });

        socket.on('answer', (data) => {
            socket.to(data.target).emit('answer', {
                sdp: data.sdp,
                sender: socket.id
            });
        });

        socket.on('ice-candidate', (data) => {
            socket.to(data.target).emit('ice-candidate', {
                candidate: data.candidate,
                sender: socket.id
            });
        });
    });
});

const PORT = process.env.PORT || 8008;
server.listen(PORT, "0.0.0.0");