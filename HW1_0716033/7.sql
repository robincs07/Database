select position, champion_name, kda
from champ P,
    (
    (select champion_id, ((sum(kills)+sum(assists))/sum(deaths)) as kda, position
    from(
        select champion_id, position, kills, assists, deaths
        from(
            select player_id, champion_id, position
            from participant
        ) P,
            (
            select player_id, kills, assists, deaths
            from stat
        ) S
        where P.player_id=S.player_id and position='DUO_CARRY'
        ) as T
    group by champion_id
    having sum(deaths)!= 0
    order by kda desc
    limit 1)
    UNION
    (select champion_id, ((sum(kills)+sum(assists))/sum(deaths)) as kda, position 
    from
(
        select champion_id, position, kills, assists, deaths
from(
            select player_id, champion_id, position
    from participant
        ) P,
    (
            select player_id, kills, assists, deaths
    from stat
        ) S
where P.player_id=S.player_id and position='DUO_CARRY'
        )
as T
    
    group by champion_id
    having sum(deaths)!= 0
    order by kda desc
    limit 1)
    UNION
(select champion_id, ((sum(kills)+sum(assists))/sum(deaths)) as kda, position
from(
        select champion_id, position, kills, assists, deaths
    from(
            select player_id, champion_id, position
        from participant
        ) P,
        (
            select player_id, kills, assists, deaths
        from stat
        ) S
    where P.player_id=S.player_id and position='DUO_SUPPORT'
        ) as T

group by champion_id
having sum(deaths)!= 0
order by kda desc
    limit 1)
    UNION
    (select champion_id,((sum(kills)+sum(assists))/sum(deaths)) as kda, position 
    from
(
        select champion_id, position, kills, assists, deaths
from(
            select player_id, champion_id, position
    from participant
        ) P,
    (
            select player_id, kills, assists, deaths
    from stat
        ) S
where P.player_id=S.player_id and position='JUNGLE'
        )
as T
    
    group by champion_id
    having sum(deaths)!= 0
    order by kda desc
    limit 1)
    UNION
(select champion_id, ((sum(kills)+sum(assists))/sum(deaths)) as kda, position
from(
        select champion_id, position, kills, assists, deaths
    from(
            select player_id, champion_id, position
        from participant
        ) P,
        (
            select player_id, kills, assists, deaths
        from stat
        ) S
    where P.player_id=S.player_id and position='MID'
        ) as T

group by champion_id
having sum(deaths)!= 0
order by kda desc
    limit 1)
    UNION
    (select champion_id,((sum(kills)+sum(assists))/sum(deaths)) as kda, position 
    from
(
        select champion_id, position, kills, assists, deaths
from(
            select player_id, champion_id, position
    from participant
        ) P,
    (
            select player_id, kills, assists, deaths
    from stat
        ) S
where P.player_id=S.player_id and position='TOP'
        )
as T
    
    group by champion_id
    having sum(deaths)!= 0
    order by kda desc
    limit 1)
) F 
where P.champion_id=F.champion_id;
