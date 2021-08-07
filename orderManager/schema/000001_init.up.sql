CREATE TABLE order
(
    id            serial       not null unique,
    fPas          varchar(255) not null,
    lPas          varchar(255) not null unique,
    track         string       not null,
    status        bool         not null
);

CREATE TABLE hub
(
    id          serial          not null unique,
    altitude    float           not null,
    longitude   float           not null
);

