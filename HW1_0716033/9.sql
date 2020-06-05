select version, win_cnt, (all_cnt-win_cnt) as lose_cnt, (win_cnt/(all_cnt)) as win_ratio
FROM
    (
select version, sum(win) as win_cnt, count(win) as all_cnt
    FROM
        (
select match_id, SUBSTRING_index(version, '.', 2) as version
        from match_info
)as match_info,
        (
select L.match_id, L.win
        from
            (select match_id, L_player_id, win
            FROM stat,
                (
select player_id as L_player_id, match_id
                from participant
                where champion_id=64
) L
            where stat.player_id=L_player_id
) as L,
            (select match_id, T_player_id, win
            FROM stat,
                (
select player_id as T_player_id, match_id
                from participant
                where champion_id=17
) T
            where stat.player_id=T_player_id
) as T
        where L.match_id=T.match_id AND L.win=T.win
) as match_win
    where match_win.match_id=match_info.match_id
    group by version
) as F;