select *
from
    (select count(win) as win
    FROM
        (
SELECT stat.player_id, win
        FROM stat,
            (
select player_id, ss1, ss2
            from participant
            where position='TOP'
                and ((ss1='Flash'and ss2='Ignite')or(ss1='Ignite'and ss2='Flash'))
) as ss
        where stat.player_id=ss.player_id
)as F_I
    where win=1) as w,
    (select count(win) as lose
    FROM
        (
SELECT stat.player_id, win
        FROM stat,
            (
select player_id, ss1, ss2
            from participant
            where position='TOP'
                and ((ss1='Flash'and ss2='Ignite')or(ss1='Ignite'and ss2='Flash'))
) as ss
        where stat.player_id=ss.player_id
)as F_I
    where win=0) as l;

select *
FROM
    (select count(win) as win
    FROM
        (
SELECT stat.player_id, win
        FROM stat,
            (
select player_id, ss1, ss2
            from participant
            where position='TOP'
                and ((ss1='Flash'and ss2='Teleport')or(ss1='Teleport'and ss2='Flash'))
) as ss
        where stat.player_id=ss.player_id
)as F_I
    where win=1) as w,
    (select count(win) as lose
    FROM
        (
SELECT stat.player_id, win
        FROM stat,
            (
select player_id, ss1, ss2
            from participant
            where position='TOP'
                and ((ss1='Flash'and ss2='Teleport')or(ss1='Teleport'and ss2='Flash'))
) as ss
        where stat.player_id=ss.player_id
)as F_I
    where win=0) as l;
