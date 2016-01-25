CREATE TABLE `guestbook` (
`id` int NOT NULL auto_increment,
`name` varchar(65) NOT NULL,
`email` varchar(65) NOT NULL,
`comment` longtext NOT NULL,
`datetime` varchar(65) NOT NULL,
PRIMARY KEY (`id`)
);
