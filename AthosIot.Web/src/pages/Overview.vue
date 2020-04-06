<template>
  <div class="content">
    <div class="container-fluid">
      <div class="row">
        <div class="col-xl-3 col-md-6">
            <card>
              <h4 slot="header" class="card-title">Links</h4>
              <div class="row">
                <div class="col-lg-12">
                  <ul list-group class="list-group">
                    <li class="list-group-item list-group-item-action flex-column align-items-start">
                      <a href="#" @click.prevent="nav(':81')">RaspAP Manager</a>
                    </li>
                    <li class="list-group-item list-group-item-action flex-column align-items-start">
                      <a href="#" @click.prevent="nav(':1880')">Node-Red</a>
                    </li>
                  </ul>
                </div>
              </div>
            </card>
        </div>
      </div>       
    </div>
  </div>
</template>
<script>
  import ChartCard from 'src/components/Cards/ChartCard.vue'
  import StatsCard from 'src/components/Cards/StatsCard.vue'
  import LTable from 'src/components/Table.vue'

  export default {
    components: {
      LTable,
      ChartCard,
      StatsCard
    },

    created() {


    },
    methods : {
      nav(path) {
        path = "http://" + window.location.hostname + path;
        window.location = path;
      }
      
    },
    
    data () {
      return {
       
      }
    }
  }
</script>
<style>

</style>
