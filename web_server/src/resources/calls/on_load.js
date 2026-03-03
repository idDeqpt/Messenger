const socket = io("http://birdplains.ru:8008");
let localStream;
let peerConnections = {};
let currentRoom = '';
let isAudioEnabled = true;
let isVideoEnabled = true;

const configuration = {
	iceServers: [
		{ urls: 'stun:stun.l.google.com:19302' },
		{ urls: 'stun:stun1.l.google.com:19302' }
	]
};

async function createRoom() {
	const roomId = generateRoomId();
	document.getElementById('room-id').value = roomId;
	console.log(roomId);
	await joinRoom();
}

function generateRoomId() {
	return Math.random().toString(36).substring(2, 8);
}

async function joinRoom() {
	const roomId = document.getElementById('room-id').value;
	if (!roomId) return;
	
	currentRoom = roomId;
	
	try {
		localStream = await navigator.mediaDevices.getDisplayMedia({
			video: true
		});
		
		addVideoStream('local', localStream);
		
		socket.emit('join-room', roomId);
		
		document.getElementById('join-screen').style.display = 'none';
		document.getElementById('conference-screen').style.display = 'block';
		
	} catch (error) {
		console.error('Ошибка доступа к медиа:', error);
		alert('Не удалось получить доступ к камере и микрофону');
	}
}

async function addVideoStream(userId, stream) {
	const videoGrid = document.getElementById('video-grid');
	
	let videoContainer = document.getElementById(`video-${userId}`);
	if (!videoContainer) {
		videoContainer = document.createElement('div');
		videoContainer.id = `video-${userId}`;
		
		const video = document.createElement('video');
		video.srcObject = stream;
		video.autoplay = true;
		video.playsInline = true;
		
		if (userId === 'local') {
			video.muted = true;
		}
		
		videoContainer.appendChild(video);
		videoGrid.appendChild(videoContainer);
	}
}

async function createPeerConnection(userId) {
	const peerConnection = new RTCPeerConnection(configuration);
	peerConnections[userId] = peerConnection;

	localStream.getTracks().forEach(track => {
		peerConnection.addTrack(track, localStream);
	});

	peerConnection.onicecandidate = (event) => {
		if (event.candidate) {
			socket.emit('ice-candidate', {
				target: userId,
				candidate: event.candidate
			});
		}
	};

	peerConnection.ontrack = (event) => {
		addVideoStream(userId, event.streams[0]);
	};

	const offer = await peerConnection.createOffer();
	await peerConnection.setLocalDescription(offer);
	
	socket.emit('offer', {
		target: userId,
		sdp: peerConnection.localDescription
	});

	return peerConnection;
}

socket.on('user-connected', async (userId) => {
	console.log('Пользователь подключился:', userId);
	await createPeerConnection(userId);
});

socket.on('offer', async (data) => {
	const peerConnection = new RTCPeerConnection(configuration);
	peerConnections[data.sender] = peerConnection;

	localStream.getTracks().forEach(track => {
		peerConnection.addTrack(track, localStream);
	});

	peerConnection.onicecandidate = (event) => {
		if (event.candidate) {
			socket.emit('ice-candidate', {
				target: data.sender,
				candidate: event.candidate
			});
		}
	};

	peerConnection.ontrack = (event) => {
		addVideoStream(data.sender, event.streams[0]);
	};

	await peerConnection.setRemoteDescription(new RTCSessionDescription(data.sdp));
	const answer = await peerConnection.createAnswer();
	await peerConnection.setLocalDescription(answer);

	socket.emit('answer', {
		target: data.sender,
		sdp: peerConnection.localDescription
	});
});

socket.on('answer', async (data) => {
	await peerConnections[data.sender].setRemoteDescription(
		new RTCSessionDescription(data.sdp)
	);
});

socket.on('ice-candidate', async (data) => {
	if (peerConnections[data.sender]) {
		await peerConnections[data.sender].addIceCandidate(
			new RTCIceCandidate(data.candidate)
		);
	}
});

socket.on('user-disconnected', (userId) => {
	if (peerConnections[userId]) {
		peerConnections[userId].close();
		delete peerConnections[userId];
	}
	
	const videoElement = document.getElementById(`video-${userId}`);
	if (videoElement) {
		videoElement.remove();
	}
});

function toggleAudio() {
	if (localStream) {
		const audioTrack = localStream.getAudioTracks()[0];
		if (audioTrack) {
			isAudioEnabled = !isAudioEnabled;
			audioTrack.enabled = isAudioEnabled;
			document.getElementById('audio-btn').textContent = 
				isAudioEnabled ? 'Выключить звук' : 'Включить звук';
		}
	}
}

function toggleVideo() {
	if (localStream) {
		const videoTrack = localStream.getVideoTracks()[0];
		if (videoTrack) {
			isVideoEnabled = !isVideoEnabled;
			videoTrack.enabled = isVideoEnabled;
			document.getElementById('video-btn').textContent = 
				isVideoEnabled ? 'Выключить видео' : 'Включить видео';
		}
	}
}

function leaveRoom() {
	if (localStream) {
		localStream.getTracks().forEach(track => track.stop());
	}
	
	Object.keys(peerConnections).forEach(userId => {
		peerConnections[userId].close();
		delete peerConnections[userId];
	});
	
	socket.disconnect();
	
	location.reload();
}