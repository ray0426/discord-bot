const Discord = require("discord.js");
const { prefix } = require("./configs/config.json");
const { token } = require("./configs/token.json");
const ytdl = require("ytdl-core-discord");
const addon = require('bindings')('addon');

const client = new Discord.Client();

const queue = new Map();

const img_data = require("./data/data.json");
const url_list = img_data.map(e => e.img_ID);
const url_amount = url_list.length;

// message when connect, disconnect
client.once("ready", () => {
	console.log("Ready!");
	addon.msg(function(msg) {
		console.log(msg);
	});
	console.log("hi1");
	addon.calc("5+3");
	console.log("hi1-1");
	console.log(typeof addon.calc("1+2*3"));
	console.log("hi2");
	console.log(addon.calc("4*6"));
});

client.once("reconnecting", () => {
	console.log("Reconnecting!");
});

client.once("disconnect", () => {
	console.log("Disconnect!");
});

// when other user type message
client.on("message", async message => {
	if (message.author.bot) return;
	if (!message.content.startsWith(prefix)) return;

	const serverQueue = queue.get(message.guild.id);

	if (message.content.startsWith(`${prefix}play`)) {
		execute(message, serverQueue);
		return;
	} else if (message.content.startsWith(`${prefix}skip`)) {
		skip(message, serverQueue);
		return;
	} else if (message.content.startsWith(`${prefix}stop`)) {
		stop(message, serverQueue);
		return;
	} else if (message.content.startsWith(`${prefix}random`)) {
		message.channel.send("Random number: " + Math.random().toString());
	} else if (message.content.startsWith(`${prefix}抽`)) {
		message.channel.send("https://www.pixiv.net/artworks/" + url_list[parseInt(Math.random()*url_amount)].toString());
	} else if (message.content.startsWith(`${prefix}calc`)) {
		message.channel.send("答案：" + addon.calc(message.content.replace(`${prefix}calc `, "")));
	}else {
		message.channel.send("You need to enter a valid command!");
	}
});

// when type "!!play message"
async function execute(message, serverQueue) {
	console.log("Get command: " + message.content);
	const args = message.content.split(" ");

	const voiceChannel = message.member.voice.channel;
	if (!voiceChannel)
		return message.channel.send(
			"You need to be in a voice channel to play music!"
		);
	const permissions = voiceChannel.permissionsFor(message.client.user);
	if (!permissions.has("CONNECT") || !permissions.has("SPEAK")) {
		return message.channel.send(
			"I need the permissions to join and speak in your voice channel!"
		);
	}

	try {
		await ytdl.getInfo(args[1]);
	} catch (err) {
		return message.channel.send(
			"You need to enter a valid youtube url!"
		);
	}
	const songInfo = await ytdl.getInfo(args[1]);
	console.log("songInfo: " + (typeof songInfo));
	const song = {
		title: songInfo.videoDetails.title,
		url: songInfo.videoDetails.video_url,
	};
	
	console.log("serverQueue: " + (typeof serverQueue));
	if (!serverQueue) {
		const queueContruct = {
			textChannel: message.channel,
			voiceChannel: voiceChannel,
			connection: null,
			songs: [],
			volume: 5,
			playing: true
		};

		queue.set(message.guild.id, queueContruct);

		queueContruct.songs.push(song); // also change queueConstruct in queue

		try {
			var connection = await voiceChannel.join();
			queueContruct.connection = connection;
			await play(message.guild, queueContruct.songs[0]);
		} catch (err) {
			console.log(err);
			queue.delete(message.guild.id);
			return message.channel.send(err);
		}
	} else {
		serverQueue.songs.push(song);
		return message.channel.send(
			`${song.title} has been added to the queue!`
		);
	}
}

// when type "!!skip"
function skip(message, serverQueue) {
	if (!message.member.voice.channel)
		return message.channel.send(
			"You have to be in a voice channel to stop the music!"
		);
	if (!serverQueue)
		return message.channel.send("There is no song that I could skip!");
	serverQueue.connection.dispatcher.end();
}

// when type "!!stop"
function stop(message, serverQueue) {
	if (!message.member.voice.channel)
		return message.channel.send(
			"You have to be in a voice channel to stop the music!"
		);
	serverQueue.songs = [];
	serverQueue.connection.dispatcher.end();
}


function play(guild, song) {
	const serverQueue = queue.get(guild.id);
	if (!song) {
		serverQueue.voiceChannel.leave();
		queue.delete(guild.id);
		return;
	}
	
	play_audio(guild, serverQueue, song.url);
	/*const dispatcher = serverQueue.connection
		.play(await ytdl(song.url, { highWaterMark: 1<<25 }), { type: 'opus'})
		.on("finish", () => {
			serverQueue.songs.shift();
			play(guild, serverQueue.songs[0]);
		})
		.on("error", error => console.error(error));*/
	//dispatcher.setVolumeLogarithmic(serverQueue.volume / 5);
	serverQueue.textChannel.send(`Start playing: **${song.title}**`);
}

async function play_audio(guild, serverQueue, url) {
	console.log("===============guild================");
	console.log(guild);
	console.log("=============serverQueue============");
	console.log(serverQueue);
	console.log("================url=================");
	console.log(url);
	const dispatcher = serverQueue.connection.play(await ytdl(url, { highWaterMark: 1<<25 }), { type: 'opus' })
		.on("finish", () => {
			serverQueue.songs.shift();
			play(guild, serverQueue.songs[0]);
		})
		.on("error", error => console.error(error));
	dispatcher.setVolumeLogarithmic(serverQueue.volume / 5);
	serverQueue.connection.dispatcher = dispatcher;
}

client.login(token);
