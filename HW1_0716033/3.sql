select champion_name, times  cnt
from champ , (
    select champion_id, count(champion_id) times
    from participant
    where position='JUNGLE'
    group by champion_id
    order by times desc
    limit 3
) as top3
where champ.champion_id=top3.champion_id;