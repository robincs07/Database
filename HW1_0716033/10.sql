SELECT self_champ_name, win_ratio, self_kda, self_avg_gold,
    champion_name as enemy_champ_name, enemy_kda, enemy_avg_gold, battle_record
FROM champ,
    (
    select champion_name as self_champ_name,
        (s_win/battle_record) as win_ratio,
        n_kda as self_kda,
        (self_s_gold/battle_record) as self_avg_gold,
        R_champion_id, R_kda as enemy_kda,
        (e_s_gold/battle_record) as enemy_avg_gold, battle_record
    FROM champ,
        (
        select n_champion_id, sum(N.win) as s_win,
            ((sum(N.kills)+sum(N.assists))/sum(N.deaths)) as n_kda,
            sum(N.goldearned) as self_s_gold, R_champion_id,
            ((sum(R.kills)+sum(R.assists))/sum(R.deaths)) as R_kda,
            sum(R.goldearned) as e_s_gold, count(n_champion_id) as battle_record
        from
            (
select match_id, win, champion_id as n_champion_id, kills, deaths, assists, goldearned
            from stat,
                (
select match_id, player_id, champion_id
                from participant
                where match_id in(
select match_id
                    from participant
                    where champion_id=58 AND position='TOP'
) and position='TOP'
) as participant
            where stat.player_id=participant.player_id and champion_id!=58
) as N,
            (
select match_id, win, champion_id as R_champion_id, kills, deaths, assists, goldearned
            from stat,
                (
select match_id, player_id, champion_id
                from participant
                where match_id in(
select match_id
                    from participant
                    where champion_id=58 AND position='TOP'
) and position='TOP'
) as participant
            where stat.player_id=participant.player_id and champion_id=58
) as R
        where N.match_id=R.match_id and N.win!=R.win
        group by n_champion_id
        having count(n_champion_id)>100 
) as F
    where champion_id=n_champion_id
) as F
where champion_id=R_champion_id
order by win_ratio desc
limit 5;