select REPLACE(REPLACE(win, 0, 'lose'), 1, 'win') as win_lose, count(avgt) as cnt
from
    (
    select win, sum(longesttimespentliving)/5 as avgt
    from participant P, stat S
    where P.player_id=S.player_id
    group by match_id, win
)as ww
where avgt>=1200
group by win;