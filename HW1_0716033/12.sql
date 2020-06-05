select *
FROM
    (
select count(win) as win
    from stat
    where firstblood=1 and win=1
) as w,
    (
select count(win) as lose
    from stat
    where firstblood=1 and win=0
) as l;

select count(*)
from
    (
select w.match_id, w.win as win, w.s_kills as w_kills, w.s_gold as w_gold,
        l.win as lose, l.s_kills as l_kills, l.s_gold as l_gold
    FROM
        (
select match_id, win, sum(kills) as s_kills, sum(goldearned) as s_gold
        FROM
            (
select match_id, stat.player_id, win, kills, goldearned
            from stat, participant
            where stat.player_id=participant.player_id
) as stat
        where win=1
        group by match_id
) as w,
        (
select match_id, win, sum(kills) as s_kills, sum(goldearned) as s_gold
        FROM
            (
select match_id, stat.player_id, win, kills, goldearned
            from stat, participant
            where stat.player_id=participant.player_id
) as stat
        where win=0
        group by match_id
) as l
    where w.match_id=l.match_id
) as F
where l_gold>w_gold;

select count(*)
from
    (
select w.match_id, w.win as win, w.s_kills as w_kills, w.s_gold as w_gold,
        l.win as lose, l.s_kills as l_kills, l.s_gold as l_gold
    FROM
        (
select match_id, win, sum(kills) as s_kills, sum(goldearned) as s_gold
        FROM
            (
select match_id, stat.player_id, win, kills, goldearned
            from stat, participant
            where stat.player_id=participant.player_id
) as stat
        where win=1
        group by match_id
) as w,
        (
select match_id, win, sum(kills) as s_kills, sum(goldearned) as s_gold
        FROM
            (
select match_id, stat.player_id, win, kills, goldearned
            from stat, participant
            where stat.player_id=participant.player_id
) as stat
        where win=0
        group by match_id
) as l
    where w.match_id=l.match_id
) as F
where l_kills>w_kills;

